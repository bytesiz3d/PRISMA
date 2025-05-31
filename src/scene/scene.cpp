#include "scene.hpp"
#include "../input/input_manager.hpp"
#include <string>
#include <fstream>
#include "../orb/orb.hpp"
#include "../mesh/mesh_utils.hpp"

#include "GLFW/glfw3.h"
using json = nlohmann::json;

GLFWwindow* Scene::window = nullptr;
Camera Scene::camera;

// ====================================================================================================
void Scene::ParseScene(Scene_Node* parent, const json& data, bool isRoot) {
  if (data.value("disabled", false) || !parent)
    return;

  const MESH_TYPE meshKey = data.value("mesh", MESH_CUBE);

  auto element = std::make_unique<Scene_Node>(meshes[meshKey]);

  const MESH_TEXTURE textureKey = data.value("texture", MESH_TEXTURE_NULL);
  element->texture = textures[textureKey];

  const OBJECT_TYPE objectKey = data.value("type", OBJECT_NULL);
  objects[objectKey].push_back(element.get());

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

  for (auto child: data.value("children", json::array())) {
    ParseScene(element.get(), child);
  }

  parent->AddChild(std::move(element));
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

Scene::Scene(const std::string& scenePath) {
  InitScene(scenePath);
}

// ====================================================================================================
void Scene::InitScene(const std::string& scenePath) {
  glm::mat4 Model;

  // Player
  player = std::make_unique<Player>(meshes[MESH_MODEL0]);
  player->position = glm::vec3(-86, 8, 0);
  player->absoluteScale = glm::vec3(16);
  //player->color = glm::vec4(0, 1, 0, 1);
  player->direction = {0, 0, 1};
  player->orientation = {0, std::asin(1), 0};
  player->texture = textures[MESH_TEXTURE_NULL];

  // Lamp
  lamp = std::make_unique<Lamp>(meshes[MESH_MODEL4]);
  lamp->position = glm::vec3(0, 130, 0);
  lamp->absoluteScale = glm::vec3(5);
  lamp->color = glm::vec4(1, 1, 1, 1);
  lamp->direction = {0, 0, 1};
  lamp->orientation = {0, std::asin(1), 0};
  lamp->texture = textures[MESH_TEXTURE_NULL];

  // HUD:
  hud = std::make_unique<Scene_Node>();
  hud->relativeModel = glm::translate(glm::mat4(1), glm::vec3(0, -0.9f, 0));

  auto primaryColor = std::make_unique<Scene_Node>(meshes[MESH_CUBE]);
  primaryColor->relativeModel = glm::translate(glm::mat4(1), glm::vec3(0.75f, 0, 0));;
  primaryColor->color = player->color;
  hud->AddChild(std::move(primaryColor));

  auto secondaryColor = std::make_unique<Scene_Node>(meshes[MESH_CUBE]);
  secondaryColor->relativeModel = glm::translate(glm::mat4(1), glm::vec3(0.85f, 0, 0));;
  hud->AddChild(std::move(secondaryColor));

  // Load the JSON file and parse it to root
  std::ifstream sceneFile(scenePath);
  json sceneData;
  sceneFile >> sceneData;

  root = std::make_unique<Scene_Node>();
  levelRooms.resize(sceneData.size());
  for (auto room: sceneData) {
    ParseScene(root.get(), room, true);
  }
}

// ====================================================================================================
std::tuple<std::unique_ptr<Scene_Node>, std::span<std::unique_ptr<Scene_Node>>> Scene::InitMainMenu(const std::string &levelsPath, const Font &font) {

  int WIDTH, HEIGHT;
  glfwGetWindowSize(Scene::getWindow(), &WIDTH, &HEIGHT);

  auto menu = std::make_unique<Scene_Node>();
  menu->worldModel = glm::ortho<float>(0, WIDTH, 0, HEIGHT);

  glm::vec3 titlePosition(HEIGHT * 0.8);

  auto title = Scene::Text("PRISMA", font, titlePosition, glm::vec4(1, 1, 0, 0.5), 2);
  menu->AddChild(std::move(title));

  // Load the JSON file and parse it
  std::ifstream levelsFile(levelsPath);
  json levels;
  levelsFile >> levels;

  float availableLevelsSpace = titlePosition.y;
  float spacing = availableLevelsSpace / (levels.size() + 2);  // 2 for space above and below the levels

  glm::vec3 levelPosition(availableLevelsSpace - (2 * spacing)); // multiply by 2 to leave space between title and first level

  std::unordered_map<int, std::pair<std::string, Scene_Node*>> levelsMap;


  for (const auto& level: levels) {
    const std::string& name = level["name"];

    auto scene = Scene::Text(name.c_str() , font, levelPosition, glm::vec4(1));
    menu->AddChild(std::move(scene));

    levelPosition.y -= spacing;
  }

  auto level_scenes = std::span(menu->children.begin() + 1, menu->children.end());

  return {std::move(menu), level_scenes};
}

// ====================================================================================================
// Upload light information to shader
void Scene::UploadLights(const Shader &shader) const {
  shader.setUniform("LightsNum", (int)lights.size());
  char location[64];
  for (GLuint i = 0; i < lights.size(); i++) {
    std::sprintf(location, "lights[%d].ambient", i);
    shader.setUniform(location, lights[i].ambient);

    std::sprintf(location, "lights[%d].diffuse", i);
    shader.setUniform(location, lights[i].diffuse);

    std::sprintf(location, "lights[%d].specular", i);
    shader.setUniform(location, lights[i].specular);

    std::sprintf(location, "lights[%d].direction", i);
    shader.setUniform(location, lights[i].direction);

    std::sprintf(location, "lights[%d].position", i);
    shader.setUniform(location, lights[i].position);

    std::sprintf(location, "lights[%d].attenuation", i);
    shader.setUniform(location, lights[i].attenuation);
  }
}

GLFWwindow * Scene::getWindow() {
  if (window == nullptr) {
    window = glfwGetCurrentContext();
  }
  return window;
}

void Scene::setWindow(GLFWwindow *newWindow) {
  window = newWindow;
  if (window) {
    glfwSetWindowSizeCallback(window, Scene::updateWindowSizeCallback);
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    camera.aspectRatio = static_cast<float>(width) / static_cast<float>(height);
  }
}

// ====================================================================================================
void Scene::UpdateData(float deltaTime) {
  auto [exit, swapColors, cameraMovement, playerMovement] = InputManager::ProcessGameInput();

  if (exit) {
    glfwSetWindowShouldClose(Scene::window, true);
  }

  player->UpdatePlayer(cameraMovement, playerMovement, deltaTime);
  lamp->UpdateLamp(getLampPosition(player->position));
  // Swap colors
  if (swapColors) {
    // Swap colors
    glm::vec4 newPlayerColor = hud->children[1]->color;
    hud->children[1]->color = player->color;
    player->color = newPlayerColor;

    // Update HUD
    hud->children[0]->color = newPlayerColor;
  }
  ProcessCollision(cameraMovement, playerMovement, deltaTime);

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
  // We need to know which axis is the door's area to use, and which is the door depth (thickness) to ignore.

  auto door_length = B_max - B_min;
  std::array<int, door_length.length()> door_axis = {0, 1, 2};
  // Sort the axes by length, so that we can ignore the smallest one (the door thickness).
  std::sort(door_axis.begin(), door_axis.end(), [&door_length](int a, int b) { return door_length[a] > door_length[b]; });

  ret = ret &&
      A_min[door_axis[0]] >= B_min[door_axis[0]] && A_max[door_axis[0]] <= B_max[door_axis[0]] &&
      A_min[door_axis[1]] >= B_min[door_axis[1]] && A_max[door_axis[1]] <= B_max[door_axis[1]];
  return ret;
}

// ====================================================================================================
void Scene::ProcessCollision(const glm::vec2& cameraMovement, const glm::vec2& playerMovement, float deltaTime) {
  for (const auto door: objects[OBJECT_DOOR]) {
    if (DoorCollide(player.get(), door)) {
      if (glm::vec3(player->color) != glm::vec3(door->color)) {
        // Revert the move and put the player one frame back
        player->UpdatePlayer(cameraMovement, -playerMovement, deltaTime);
        player->UpdatePlayer(cameraMovement, -playerMovement, deltaTime);
      }
      return;
    }
  }

  for (const auto wall: objects[OBJECT_WALL]) {
    if (Collide(player.get(), wall)) {
      // std::cout << "debug: hit wall\n";

      // Revert the move and put the player one frame back
      player->UpdatePlayer(-cameraMovement, -playerMovement, deltaTime);
      player->UpdatePlayer(-cameraMovement, -playerMovement, deltaTime);

      return;
    }
  }

  for (auto sn_orb: objects[OBJECT_ORB]) {
    Orb* orb = static_cast<Orb*>(sn_orb);
    if (!orb)
      continue;

    orb->currentState = Collide(player.get(), orb);

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
void Scene::DrawScene(const Scene_Node& scene, const Shader &shader) {
  scene.Draw(shader);
  for (const auto& child: scene.children)
    DrawScene(*child, shader);
}


std::unique_ptr<Scene_Node> Scene::Text(const char* string,
                        const Font& font,
                        const glm::vec2 position,
                        const glm::vec4 color,
                        const float scale,
                        const bool center_x) {
  // Create the mesh
  auto textMesh = Mesh_Utils::TextMesh(string, font);
  auto pos = glm::vec3(position.x, position.y, 0.f);

  // Center the text if required
  if (center_x) {
    int WIDTH, HEIGHT;
    glfwGetWindowSize(getWindow(), &WIDTH, &HEIGHT);

    const auto x_center = WIDTH / 2.f;
    const float offset = scale * (textMesh->AABB_max.x -textMesh->AABB_min.x) / 2.f;
    pos.x = x_center - offset;
  }

  // Create the scene node
  auto textNode = std::make_unique<Scene_Node>(textMesh);
  textNode->absoluteScale = glm::vec3(scale, scale, 1.f);
  textNode->relativeModel = glm::translate(glm::mat4(1), pos);
  textNode->color = color;

  return textNode;
}

