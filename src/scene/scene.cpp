#include <glad/glad.h>
#include "GLFW/glfw3.h"
using json = nlohmann::json;
#include <string>

// ====================================================================================================
void Scene::ParseScene(Scene_Node* parent, const json& data, bool isRoot) {
  if (data.value("disabled", false) || !parent)
    return;

  const MESH_TYPE meshKey = data.value("mesh", MESH_CUBE);

  Scene_Node* element = new Scene_Node(meshes[meshKey]);

  const MESH_TEXTURE textureKey = data.value("texture", MESH_TEXTURE_NULL);
  element->texture = textures[textureKey];

  const OBJECT_TYPE objectKey = data.value("type", OBJECT_NULL);
  objects[objectKey].push_back(element);

  // Read available data
  const auto translate = data.value("translate", std::array{0.f, 0.f, 0.f});
  auto translate_vec = glm::make_vec3(translate.data());
  element->relativeModel = glm::translate(glm::mat4(1), translate_vec);

  auto absoluteScale = data.value("absoluteScale", std::array{1.f, 1.f, 1.f});
  element->absoluteScale = glm::make_vec3(absoluteScale.data());

  auto color = data.value("color", std::array{1.f, 1.f, 1.f, 1.f});
  element->color = glm::make_vec4(color.data());

  element->drawMode = data.value("drawMode", DRAW_TRIANGLES);

  // Light:
  if (data.find("light") != data.end()) {
    typedef std::array<float, 3> vec3;

    auto light = data["light"];

    Light current_light;
    current_light.ambient = glm::make_vec3(light["ambient"].get<vec3>().data());
    current_light.diffuse = glm::make_vec3(light["diffuse"].get<vec3>().data());
    current_light.specular = glm::make_vec3(light["specular"].get<vec3>().data());
    current_light.position = glm::make_vec3(light["position"].get<vec3>().data());
    current_light.attenuation = light["attenuation"].get<float>();
    current_light.direction = glm::normalize(glm::make_vec3(light["direction"].get<vec3>().data()));

    lights.push_back(current_light);
  }

  if (isRoot) {
    levelRooms.push_back(translate_vec);
  }

  parent->AddChild(element);

  for (auto child: data.value("children", json::array())) {
    ParseScene(element, child);
  }
}

// We only have one lamp, and we move it to the nearest room (where the player is)
glm::vec3 Scene::getLampPosition(glm::vec3 playerPos) {
  glm::vec3 nearestRoom(0);
  float minDist = std::numeric_limits<float>::max();

  for (const auto& room: levelRooms) {
    auto distance = glm::distance(playerPos, room);
      if (distance < minDist) {
        minDist = distance;
        nearestRoom = room;
      }
  }

  nearestRoom.y = 130;
  return nearestRoom;
}

// ====================================================================================================
void Scene::InitScene(const std::string& scenePath) {
  glm::mat4 Model;

  // Player
  player = new Player(meshes[MESH_MODEL0]);
  player->position = glm::vec3(-86, 8, 0);
  player->absoluteScale = glm::vec3(16);
  //player->color = glm::vec4(0, 1, 0, 1);
  player->direction = {0, 0, 1};
  player->orientation = {0, std::asin(1), 0};
  player->texture = textures[MESH_TEXTURE_NULL];

  // Lamp
  lamp = new Lamp(meshes[MESH_MODEL4]);
  lamp->position = glm::vec3(0, 130, 0);
  lamp->absoluteScale = glm::vec3(5);
  lamp->color = glm::vec4(1, 1, 1, 1);
  lamp->direction = {0, 0, 1};
  lamp->orientation = {0, std::asin(1), 0};
  lamp->texture = textures[MESH_TEXTURE_NULL];

  // HUD:
  hud = new Scene_Node;
  hud->relativeModel = glm::translate(glm::mat4(1), glm::vec3(0, -0.9f, 0));

  Scene_Node* primaryColor = new Scene_Node(meshes[MESH_CUBE]);
  Model = glm::translate(glm::mat4(1), glm::vec3(0.75f, 0, 0));
  primaryColor->relativeModel = Model;
  primaryColor->color = player->color;
  hud->AddChild(primaryColor);

  Scene_Node* secondaryColor = new Scene_Node(meshes[MESH_CUBE]);
  Model = glm::translate(glm::mat4(1), glm::vec3(0.85f, 0, 0));
  secondaryColor->relativeModel = Model;
  hud->AddChild(secondaryColor);

  // Load the JSON file and parse it to root
  std::ifstream sceneFile(scenePath);
  json sceneData;
  sceneFile >> sceneData;

  root = new Scene_Node;
  levelRooms.resize(sceneData.size());
  for (auto room: sceneData) {
    ParseScene(root, room, true);
  }
}

// ====================================================================================================
// Upload light information to shader
void Scene::UploadLights(GLuint shaderID) {
  int size = glGetUniformLocation(shaderID, "LightsNum");
  glUniform1i(size, lights.size());
  char location[64];
  for (GLuint i = 0; i < lights.size(); i++) {
    std::sprintf(location, "lights[%d].ambient", i);
    int amb = glGetUniformLocation(shaderID, location);

    std::sprintf(location, "lights[%d].diffuse", i);
    int dif = glGetUniformLocation(shaderID, location);

    std::sprintf(location, "lights[%d].specular", i);
    int spc = glGetUniformLocation(shaderID, location);

    std::sprintf(location, "lights[%d].direction", i);
    int dir = glGetUniformLocation(shaderID, location);

    std::sprintf(location, "lights[%d].position", i);
    int pos = glGetUniformLocation(shaderID, location);

    std::sprintf(location, "lights[%d].attenuation", i);
    int att = glGetUniformLocation(shaderID, location);

    glUniform3f(amb, lights[i].ambient.r, lights[i].ambient.g, lights[i].ambient.b);
    glUniform3f(dif, lights[i].diffuse.r, lights[i].diffuse.g, lights[i].diffuse.b);
    glUniform3f(spc, lights[i].specular.r, lights[i].specular.g, lights[i].specular.b);
    glUniform3f(dir, lights[i].direction.x, lights[i].direction.y, lights[i].direction.z);
    glUniform3f(pos, lights[i].position.x, lights[i].position.y, lights[i].position.z);
    glUniform1f(att, lights[i].attenuation);
  }
}
// ====================================================================================================
void Scene::UpdateData() {
  glfwGetCursorPos(window, &mX, &mY);
  mouseDelta = glm::vec2(mX - p_mX, mY - p_mY);
  p_mX = mX, p_mY = mY;

  // Move player:
  dm = movementP - movementN;
  player->UpdatePlayer(mouseDelta, dm);
  lamp->UpdateLamp(getLampPosition(player->position));
  // Swap colors
  if (movementP[1] > 0) {
    if ((bool)movementP[1] != swapped) {
      // Swap colors
      glm::vec4 newPlayerColor = hud->children[1]->color;
      hud->children[1]->color = player->color;
      player->color = newPlayerColor;

      // Update HUD
      hud->children[0]->color = newPlayerColor;
      swapped = true;
    }
  }
  else
    swapped = false;

  ProcessCollision();

  // Manually updating the camera
  glm::vec4 newCameraPosition(0, 0, 0, 1);
  glm::mat4 cameraTransform(1);
  cameraTransform = glm::translate(cameraTransform, player->position);
  cameraTransform = glm::rotate(cameraTransform, player->pitch, player->normal);
  cameraTransform = glm::translate(cameraTransform, -40.f * player->direction);
  newCameraPosition = cameraTransform * newCameraPosition;

  camera.position = glm::vec3(newCameraPosition);
  camera.SetTarget(player->position);
}

// ====================================================================================================
static auto getCollisionVectors(Scene_Node* objectA, Scene_Node* objectB) {
  // AABB:
  // Get the minimum (x, y, z) and the maximum (x, y, z) for both shapes.
  // If the ranges intersect in all three axes,
  // then the shapes collide.

  // ==================================== A ====================================
  glm::vec3 A_min(1000), A_max(-1000);
  glm::mat4 A_world = objectA->ScaleWorldModel();

  glm::vec4 A_AABB_min = {objectA->mesh->AABB_min, 1.0f};
  glm::vec4 A_AABB_max = {objectA->mesh->AABB_max, 1.f};

  glm::vec4 A_AABB_vertices[8] = {
    A_AABB_min,
    {A_AABB_max[0], A_AABB_min[1], A_AABB_min[2], 1},
    {A_AABB_max[0], A_AABB_max[1], A_AABB_min[2], 1},
    {A_AABB_min[0], A_AABB_max[1], A_AABB_min[2], 1},
    A_AABB_max,
    {A_AABB_min[0], A_AABB_max[1], A_AABB_max[2], 1},
    {A_AABB_min[0], A_AABB_min[1], A_AABB_max[2], 1},
    {A_AABB_max[0], A_AABB_min[1], A_AABB_max[2], 1},
  };

  // ==================================== B ====================================
  glm::vec3 B_min(1000), B_max(-1000);
  glm::mat4 B_world = objectB->ScaleWorldModel();

  glm::vec4 B_AABB_min = {objectB->mesh->AABB_min, 1.f};
  glm::vec4 B_AABB_max = {objectB->mesh->AABB_max, 1.f};

  glm::vec4 B_AABB_vertices[8] = {
    B_AABB_min,
    {B_AABB_max[0], A_AABB_min[1], A_AABB_min[2], 1},
    {B_AABB_max[0], A_AABB_max[1], A_AABB_min[2], 1},
    {B_AABB_min[0], A_AABB_max[1], A_AABB_min[2], 1},
    B_AABB_max,
    {B_AABB_min[0], A_AABB_max[1], A_AABB_max[2], 1},
    {B_AABB_min[0], A_AABB_min[1], A_AABB_max[2], 1},
    {B_AABB_max[0], A_AABB_min[1], A_AABB_max[2], 1},
  };

  glm::vec3 transformed(0);
  for (GLubyte i = 0; i < 8; i++) {
    // A:
    transformed = A_world * A_AABB_vertices[i];
    A_min = glm::min(A_min, transformed);
    A_max = glm::max(A_max, transformed);

    // B:
    transformed = B_world * B_AABB_vertices[i];
    B_min = glm::min(B_min, transformed);
    B_max = glm::max(B_max, transformed);
  }

  return std::make_tuple(A_min, A_max, B_min, B_max);
}

bool Scene::Collide(Scene_Node *objectA, Scene_Node *objectB) {
  const auto& [A_min, A_max, B_min, B_max] = getCollisionVectors(objectA, objectB);
  // Check if the ranges intersect in all three axes
  return glm::all(glm::lessThanEqual(A_min, B_max)) && glm::all(glm::lessThanEqual(B_min, A_max));
}

bool Scene::DoorCollide(Scene_Node *objectA, Scene_Node *door) {
  const auto& [A_min, A_max, B_min, B_max] = getCollisionVectors(objectA, door);
  // Check if the ranges intersect in all three axes
  bool ret = glm::all(glm::lessThanEqual(A_min, B_max)) && glm::all(glm::lessThanEqual(B_min, A_max));

  // Make sure that object is fully inside the door to be able to pass through.
  // We check if the min and max of A are within the min and max of B.
  // This is a stricter condition than just checking for intersection.
  // We don't check x-axis because it represents the thickness of the door, which is not relevant.
  ret = ret &&
      A_min.y >= B_min.y && A_max.y <= B_max.y &&
      A_min.z >= B_min.z && A_max.z <= B_max.z;
  return ret;
}

// ====================================================================================================
void Scene::ProcessCollision() {
  for (const auto door: objects[OBJECT_DOOR]) {
    if (DoorCollide(player, door)) {
      if (glm::vec3(player->color) != glm::vec3(door->color)) {
        // Revert the move and put the player one frame back
        player->UpdatePlayer(mouseDelta, -dm);
        player->UpdatePlayer(mouseDelta, -dm);
      }
      return;
    }
  }

  for (const auto wall: objects[OBJECT_WALL]) {
    if (Collide(player, wall)) {
      // std::cout << "debug: hit wall\n";

      // Revert the move and put the player one frame back
      player->UpdatePlayer(-mouseDelta, -dm);
      player->UpdatePlayer(-mouseDelta, -dm);

      return;
    }
  }

  for (auto sn_orb: objects[OBJECT_ORB]) {
    Orb* orb = static_cast<Orb*>(sn_orb);
    if (!orb)
      continue;

    orb->currentState = Collide(player, orb);

    if (orb->currentState && (orb->currentState != orb->lastState)) {
      // Swap secondary color
      glm::vec4 new2ndColor = orb->color;
      orb->color = hud->children[1]->color;
      hud->children[1]->color = new2ndColor;

      orb->lastState = orb->currentState;
      return;
    }

    orb->lastState = orb->currentState;
  }
}

// ====================================================================================================
void Scene::DrawScene(Scene_Node* scene, GLuint shaderId) {
  if (scene) {
    scene->Draw(shaderId);
    for (auto& child: scene->children)
      DrawScene(child, shaderId);
  }
}

// ====================================================================================================
// Is called whenever a key is pressed/released via GLFW
void Scene::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);

  if (action == GLFW_PRESS) {
    switch (key) {
    case GLFW_KEY_W:
    case GLFW_KEY_UP:
      movementP[2] = 1;
      break;
    case GLFW_KEY_S:
    case GLFW_KEY_DOWN:
      movementN[2] = 1;
      break;

    case GLFW_KEY_D:
    case GLFW_KEY_RIGHT:
      movementP[0] = 1;
      break;
    case GLFW_KEY_A:
    case GLFW_KEY_LEFT:
      movementN[0] = 1;
      break;

    case GLFW_KEY_Q:
      movementP[1] = 1;
      break;
    case GLFW_KEY_E:
      movementN[1] = 1;
      break;

    case GLFW_KEY_1:
      level = 1;
      break;

    case GLFW_KEY_2:
      level = 2;
      break;

    default:
      break;
    }
  }

  if (action == GLFW_RELEASE) {
    switch (key) {
    case GLFW_KEY_W:
    case GLFW_KEY_UP:
      movementP[2] = 0;
      break;
    case GLFW_KEY_S:
    case GLFW_KEY_DOWN:
      movementN[2] = 0;
      break;

    case GLFW_KEY_D:
    case GLFW_KEY_RIGHT:
      movementP[0] = 0;
      break;
    case GLFW_KEY_A:
    case GLFW_KEY_LEFT:
      movementN[0] = 0;
      break;

    case GLFW_KEY_Q:
      movementP[1] = 0;
      break;
    case GLFW_KEY_E:
      movementN[1] = 0;
      break;

    default:
      break;
    }
  }
}


// ====================================================================================================
void Scene::DeleteAllPointers() {
  if (root)
    delete root;

  if (player)
    delete player;
}
