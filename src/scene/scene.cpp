#include <glad/glad.h>
#include "GLFW/glfw3.h"
using json = nlohmann::json;
#include <string>

// ====================================================================================================
void Scene::ParseScene(Scene_Node* parent, const json& data) {
  if (data.find("comment") != data.end() || !parent)
    return;

  Scene_Node* element;

  Mesh* mesh = meshes[MESH_CUBE];
  if (data.find("mesh") != data.end()) {
    MESH_TYPE meshKey = data["mesh"].get<MESH_TYPE>();
    if (meshes.find(meshKey) != meshes.end())
      mesh = meshes[meshKey];
  }

  Texture* texture = nullptr;
  if (data.find("texture") != data.end()) {
    MESH_TEXTURE textureKey = (MESH_TEXTURE)data["texture"].get<int>();
    if (textures[textureKey] != 0)
      texture = textures[textureKey];
  }
  else
    texture = textures[MESH_TEXTURE_NULL];

  // Fill the object vectors
  if (data.find("type") != data.end()) {
    OBJECT_TYPE type = data["type"].get<OBJECT_TYPE>();
    switch (type)
    // switch ((int)data["type"])
    {
    case OBJECT_DOOR:
      element = new Scene_Node(mesh);
      doors.push_back(element);
      break;

    case OBJECT_WALL:
      element = new Scene_Node(mesh);
      walls.push_back(element);
      break;

    case OBJECT_ORB:
      element = new Orb(mesh);
      orbs.push_back(element);
      break;

    default:
      element = new Scene_Node(mesh);
    }
  }
  else
    element = new Scene_Node(mesh);

  parent->AddChild(element);
  element->texture = texture;

  // Read available data
  if (data.find("relativeModel") != data.end()) {
    auto relativeModel = data["relativeModel"].get<std::vector<float>>();
    element->relativeModel = glm::make_mat4(&relativeModel[0]);
    element->relativeModel = glm::transpose(element->relativeModel);
  }

  if (data.find("absoluteScale") != data.end()) {
    auto absoluteScale = data["absoluteScale"].get<std::vector<float>>();
    element->absoluteScale = glm::make_vec3(&absoluteScale[0]);
  }

  if (data.find("color") != data.end()) {
    auto color = data["color"].get<std::vector<float>>();
    element->color = glm::make_vec4(&color[0]);
  }

  if (data.find("drawMode") != data.end()) {
    DRAW_MODE mode = data["drawMode"].get<DRAW_MODE>();
    element->drawMode = mode;
  }

  // Light:
  if (data.find("light") != data.end()) {
    // Read data from file and append to vector
    auto ambient = data["light"]["ambient"].get<std::vector<float>>();
    auto diffuse = data["light"]["diffuse"].get<std::vector<float>>();
    auto specular = data["light"]["specular"].get<std::vector<float>>();
    auto direction = data["light"]["direction"].get<std::vector<float>>();
    auto position = data["light"]["position"].get<std::vector<float>>();
    auto attenuation = data["light"]["attenuation"].get<float>();

    Light current_light;
    current_light.ambient = glm::make_vec3(ambient.data());
    current_light.diffuse = glm::make_vec3(diffuse.data());
    current_light.specular = glm::make_vec3(specular.data());
    current_light.direction = glm::make_vec3(direction.data());
    current_light.position = glm::make_vec3(position.data());
    current_light.attenuation = attenuation;

    current_light.direction = glm::normalize(current_light.direction);
    lights.push_back(current_light);
  }

  if (data.find("children") != data.end()) {
    for (auto child: data["children"])
      ParseScene(element, child);
  }
}

glm::vec3 Scene::getNearestRoomPositionLv2(glm::vec3 playerPos) {
  int indexOfMin = 0;
  float minXZ = 3000;

  for (int i = 0; i < 9; i++) {
    float Px = playerPos[0];
    float Pz = playerPos[2];
    float Rx = levelTwoRoomsX[i];
    float Rz = levelTwoRoomsZ[i];

    float dist = sqrt((Px - Rx) * (Px - Rx) + (Pz - Rz) * (Pz - Rz));
    if (minXZ < dist) {
      minXZ = minXZ;
    }
    else {
      minXZ = dist;
      indexOfMin = i;
    }
  }
  glm::vec3 roomPos;
  roomPos[0] = levelTwoRoomsX[indexOfMin];
  roomPos[2] = levelTwoRoomsZ[indexOfMin];
  roomPos[1] = 130;
  return roomPos;
}
glm::vec3 Scene::getNearestRoomPositionLv1(glm::vec3 playerPos) {
  int indexOfMin = 0;
  float minXZ = 3000;

  for (int i = 0; i < 7; i++) {
    float Px = playerPos[0];
    float Pz = playerPos[2];
    float Rx = levelOneRoomsX[i];
    float Rz = levelOneRoomsZ[i];

    float dist = sqrt((Px - Rx) * (Px - Rx) + (Pz - Rz) * (Pz - Rz));
    if (minXZ < dist) {
      minXZ = minXZ;
    }
    else {
      minXZ = dist;
      indexOfMin = i;
    }
  }
  glm::vec3 roomPos;
  roomPos[0] = levelOneRoomsX[indexOfMin];
  roomPos[2] = levelOneRoomsZ[indexOfMin];
  roomPos[1] = 130;
  return roomPos;
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
  for (auto element: sceneData) {
    //std::cout << element.type_name();
    ParseScene(root, element);
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

  if (!holdMouse)
    mouseDelta = glm::vec2(0);

  // Move player:
  dm = movementP - movementN;
  player->UpdatePlayer(mouseDelta, dm);
  glm::vec3 lampPosition;
  if (level == 1) {
    lampPosition = getNearestRoomPositionLv1(player->position);
  }
  else {
    lampPosition = getNearestRoomPositionLv2(player->position);
  }
  lamp->UpdateLamp(lampPosition);
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
bool Scene::Collide(Scene_Node* objectA, Scene_Node* objectB) {
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

  return (
    A_min[0] <= B_max[0] && B_min[0] <= A_max[0] &&
    A_min[1] <= B_max[1] && B_min[1] <= A_max[1] &&
    A_min[2] <= B_max[2] && B_min[2] <= A_max[2]);
}

// ====================================================================================================
void Scene::ProcessCollision() {
  for (auto door: doors) {
    if (Collide(player, door)) {
      if (glm::vec3(player->color) != glm::vec3(door->color)) {
        // Revert the move and put the player one frame back
        player->UpdatePlayer(mouseDelta, -dm);
        player->UpdatePlayer(mouseDelta, -dm);
      }
      return;
    }
  }

  for (auto wall: walls) {
    if (Collide(player, wall)) {
      // std::cout << "debug: hit wall\n";

      // Revert the move and put the player one frame back
      player->UpdatePlayer(-mouseDelta, -dm);
      player->UpdatePlayer(-mouseDelta, -dm);

      return;
    }
  }

  for (auto sn_orb: orbs) {
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
void Scene::MouseCallback(GLFWwindow* window, int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT) {
    if (action == GLFW_PRESS)
      holdMouse = true;
    else if (action == GLFW_RELEASE)
      holdMouse = false;
  }
}

// ====================================================================================================
void Scene::DeleteAllPointers() {
  if (root)
    delete root;

  if (player)
    delete player;
}
