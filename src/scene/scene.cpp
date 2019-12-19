using json = nlohmann::json;

// ====================================================================================================
void Scene::ParseScene(Scene_Node* parent, const json& data)
{   
    if (data.find("comment") != data.end() || !parent)
        return;

    Scene_Node* element;

    Mesh* mesh = meshes[MESH_CUBE];
    if (data.find("mesh") != data.end())
    {
        MESH_TYPE meshKey = data["mesh"].get<MESH_TYPE>();
        if (meshes[meshKey] != 0)
            mesh = meshes[meshKey];
    }

    Texture* texture = nullptr;
    if (data.find("texture") != data.end())
    {
        MESH_TEXTURE textureKey = (MESH_TEXTURE)data["texture"].get<int>();
        if (textures[textureKey] != 0)
            texture = textures[textureKey];
    }
    else
        texture = textures[MESH_TEXTURE_NULL];

    // Fill the object vectors
    if (data.find("type") != data.end())
    {
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
    if (data.find("relativeModel") != data.end())
    {
        auto relativeModel = data["relativeModel"].get<std::vector<float>>();
        element->relativeModel = glm::make_mat4(&relativeModel[0]);
        element->relativeModel = glm::transpose(element->relativeModel);
    }

    if (data.find("absoluteScale") != data.end())
    {
        auto absoluteScale = data["absoluteScale"].get<std::vector<float>>();
        element->absoluteScale = glm::make_vec3(&absoluteScale[0]);
    }

    if (data.find("color") != data.end())
    {
        auto color = data["color"].get <std::vector<float>>();
        element->color = glm::make_vec4(&color[0]);
    }

    if (data.find("drawMode") != data.end())
    {
        DRAW_MODE mode = data["drawMode"].get<DRAW_MODE>();
        element->drawMode = mode;
    }

    if (data.find("children") != data.end())
    {
        for (auto child: data["children"])
            ParseScene(element, child);
    }
}

// ====================================================================================================
void Scene::InitScene(const std::string& scenePath)
{
    glm::mat4 Model;

    // Player
    player = new Player(meshes[MESH_MODEL0]);
    player->position = glm::vec3(-64, 8, 0);
    player->absoluteScale = glm::vec3(8);
    player->color = glm::vec4(0, 1, 0, 1);
    player->orientation = { std::asin(0.3), std::asin(1), 0 };
    player->texture = textures[MESH_TEXTURE_NULL];

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
    for (auto element: sceneData)
    {
        //std::cout << element.type_name();
        ParseScene(root, element);
    }
}

// ====================================================================================================
void Scene::UpdateData()
{
    glfwGetCursorPos(window, &mX, &mY);
    mouseDelta = glm::vec2(mX - p_mX, mY - p_mY);
    p_mX = mX, p_mY = mY;

    if (!holdMouse)
        mouseDelta = glm::vec2(0);

    // Move player:
    dm = movementP - movementN;
    player->UpdatePlayer(mouseDelta, dm);

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
bool Scene::Collide(Scene_Node* objectA, Scene_Node* objectB)
{
    // AABB:
    // Get the minimum (x, y, z) and the maximum (x, y, z) for both shapes.
    // If the ranges intersect in all three axes,
    // then the shapes collide.

    // ==================================== A ====================================
    glm::vec3 A_min(1000), A_max(-1000);
    glm::mat4 A_world = objectA->ScaleWorldModel();

    glm::vec4 A_AABB_min = {
        objectA->mesh->AABB_min[0],
        objectA->mesh->AABB_min[1],
        objectA->mesh->AABB_min[2],
        1.f
    };
    glm::vec4 A_AABB_max = {
        objectA->mesh->AABB_max[0],
        objectA->mesh->AABB_max[1],
        objectA->mesh->AABB_max[2],
        1.f
    };

    glm::vec4 A_AABB_vertices[8] = {
        A_AABB_min,
        { A_AABB_max[0],  A_AABB_min[1], A_AABB_min[2], 1 },
        { A_AABB_max[0],  A_AABB_max[1], A_AABB_min[2], 1 },
        { A_AABB_min[0],  A_AABB_max[1], A_AABB_min[2], 1 },
        A_AABB_max,
        { A_AABB_min[0],  A_AABB_max[1], A_AABB_max[2], 1 },
        { A_AABB_min[0],  A_AABB_min[1], A_AABB_max[2], 1 },
        { A_AABB_max[0],  A_AABB_min[1], A_AABB_max[2], 1 },
    };

    // ==================================== B ====================================
    glm::vec3 B_min(1000), B_max(-1000);
    glm::mat4 B_world = objectB->ScaleWorldModel();

    glm::vec4 B_AABB_min = {
        objectB->mesh->AABB_min[0],
        objectB->mesh->AABB_min[1],
        objectB->mesh->AABB_min[2],
        1.f
    };
    glm::vec4 B_AABB_max = {
        objectB->mesh->AABB_max[0],
        objectB->mesh->AABB_max[1],
        objectB->mesh->AABB_max[2],
        1.f
    };

    glm::vec4 B_AABB_vertices[8] = {
        B_AABB_min,
        { B_AABB_max[0],  A_AABB_min[1], A_AABB_min[2], 1 },
        { B_AABB_max[0],  A_AABB_max[1], A_AABB_min[2], 1 },
        { B_AABB_min[0],  A_AABB_max[1], A_AABB_min[2], 1 },
        B_AABB_max,
        { B_AABB_min[0],  A_AABB_max[1], A_AABB_max[2], 1 },
        { B_AABB_min[0],  A_AABB_min[1], A_AABB_max[2], 1 },
        { B_AABB_max[0],  A_AABB_min[1], A_AABB_max[2], 1 },
    };
    

    glm::vec4 transformed(0);
    for (GLubyte i = 0; i < 8; i++)
    {
        // A:
        transformed = A_world * A_AABB_vertices[i];

        A_min[0] = transformed[0] < A_min[0] ? transformed[0] : A_min[0];
        A_min[1] = transformed[1] < A_min[1] ? transformed[1] : A_min[1];
        A_min[2] = transformed[2] < A_min[2] ? transformed[2] : A_min[2];
        
        A_max[0] = transformed[0] > A_max[0] ? transformed[0] : A_max[0];
        A_max[1] = transformed[1] > A_max[1] ? transformed[1] : A_max[1];
        A_max[2] = transformed[2] > A_max[2] ? transformed[2] : A_max[2];

        // B:
        transformed = B_world * B_AABB_vertices[i];

        B_min[0] = transformed[0] < B_min[0] ? transformed[0] : B_min[0];
        B_min[1] = transformed[1] < B_min[1] ? transformed[1] : B_min[1];
        B_min[2] = transformed[2] < B_min[2] ? transformed[2] : B_min[2];

        B_max[0] = transformed[0] > B_max[0] ? transformed[0] : B_max[0];
        B_max[1] = transformed[1] > B_max[1] ? transformed[1] : B_max[1];
        B_max[2] = transformed[2] > B_max[2] ? transformed[2] : B_max[2];
    }
    
    return (
        A_min[0] <= B_max[0] && B_min[0] <= A_max[0] &&
        A_min[1] <= B_max[1] && B_min[1] <= A_max[1] &&
        A_min[2] <= B_max[2] && B_min[2] <= A_max[2] 
    );
}

// ====================================================================================================
void Scene::ProcessCollision()
{
    for (auto door: doors)
    {
        if (Collide(player, door))
        {
            if (player->color != door->color)
            {
                // Revert the move and put the player one frame back
                player->UpdatePlayer(mouseDelta, -dm);
                player->UpdatePlayer(mouseDelta, -dm);
            }
            return;
        }
    }

    for (auto wall: walls)
    {
        if (Collide(player, wall))
        {
            // std::cout << "debug: hit wall\n";

            // Revert the move and put the player one frame back
            player->UpdatePlayer(-mouseDelta, -dm);
            player->UpdatePlayer(-mouseDelta, -dm);

            return;
        }
    }

    for (auto sn_orb: orbs)
    {
        Orb* orb = static_cast<Orb*>(sn_orb);
        if (!orb)
            continue;
         
        orb->currentState = Collide(player, orb);

        if (orb->currentState && (orb->currentState != orb->lastState))
        {
            // Swap colors
            glm::vec4 newPlayerColor = orb->color;
            orb->color = player->color;
            player->color = newPlayerColor;

            // Update HUD
            hud->children[0]->color = newPlayerColor;
            
            orb->lastState = orb->currentState;
            return;
        }

        orb->lastState = orb->currentState;
    }
}

// ====================================================================================================
void Scene::DrawScene(Scene_Node* scene, GLuint shaderId)
{
    if (scene)
    {
        scene->Draw(shaderId);
        for (auto& child : scene->children)
            DrawScene(child, shaderId);
    }
}

// ====================================================================================================
// Is called whenever a key is pressed/released via GLFW
void Scene::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_W:
            movementP[2] = 1;
            break;
        case GLFW_KEY_S:
            movementN[2] = 1;
            break;

        case GLFW_KEY_D:
            movementP[0] = 1;
            break;
        case GLFW_KEY_A:
            movementN[0] = 1;
            break;

        case GLFW_KEY_Q:
            movementP[1] = 1;
            break;
        case GLFW_KEY_E:
            movementN[1] = 1;
            break;

        default:
            break;
        }
    }

    if (action == GLFW_RELEASE)
    {
        switch (key)
        {
        case GLFW_KEY_W:
            movementP[2] = 0;
            break;
        case GLFW_KEY_S:
            movementN[2] = 0;
            break;

        case GLFW_KEY_D:
            movementP[0] = 0;
            break;
        case GLFW_KEY_A:
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
void Scene::MouseCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action == GLFW_PRESS)
            holdMouse = true;
        else if (action == GLFW_RELEASE)
            holdMouse = false;
    }
}

// ====================================================================================================
void Scene::DeleteAllPointers()
{
    if (root)
        delete root;

    if (player)
        delete player;

}
