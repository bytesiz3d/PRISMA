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

    if (Collide(player, door))
    {
        if (player->color != door->color)
        {
            // Revert the move and put the player one frame back
            player->UpdatePlayer(mouseDelta, -dm);
            player->UpdatePlayer(mouseDelta, -dm);
        }
    }
    else
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

    // Check for collision with the orb
    bool currentState = Collide(player, orb);
    if (currentState != lastState && currentState)
    {
        // Swap colors
        glm::vec4 newPlayerColor = orb->color;
        orb->color = player->color;
        player->color = newPlayerColor;

        // Update HUD
        hud->children[0]->color = newPlayerColor;
    }
    lastState = currentState;
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
    for (auto wall : walls)
    {
        // // Put either before the bottom floor to prevent collision testing against it
        // if (wall == door || wall == orb)
        //     return;

        if (Collide(player, wall))
        {
            std::cout << "debug: hit wall\n";
            // Revert the move and put the player one frame back
            player->UpdatePlayer(-mouseDelta, -dm);
            player->UpdatePlayer(-mouseDelta, -dm);
        }
    }
}

// ====================================================================================================
void Scene::InitScene(Mesh* cc, Mesh* pp)
{
    glm::mat4 Model;
    room = new Scene_Node;

    // Player
    player = new Player(pp);
    player->position = glm::vec3(-64, 8, 0);
    player->absoluteScale = glm::vec3(8);
    player->color = glm::vec4(0, 1, 0, 1);
    player->orientation = { std::asin(0.3), std::asin(1), 0 };
        
    // Right face:
    Scene_Node* right = new Scene_Node(cc);
    Model = glm::translate(glm::mat4(1), glm::vec3(128, 0, 0));
    right->relativeModel = Model;
    right->absoluteScale = glm::vec3(000, 256, 256); 
    room->AddChild(right);
    walls.push_back(right);
    
    // Left face:
    Scene_Node* left = new Scene_Node(cc);
    Model = glm::translate(glm::mat4(1), glm::vec3(-128, 0, 0));
    left->relativeModel = Model;
    left->absoluteScale = glm::vec3(000, 256, 256); 
    room->AddChild(left);
    walls.push_back(left);

    // Front face:
    Scene_Node* front = new Scene_Node(cc);
    Model = glm::translate(glm::mat4(1), glm::vec3(0, 0, -128));
    front->relativeModel = Model;
    front->absoluteScale = glm::vec3(256, 256, 000);
    front->color = glm::vec4(21.f/255, 5.f/255, 42.f/255, 1.f);
    room->AddChild(front);
    walls.push_back(front);

    // Back face:
    Scene_Node* back = new Scene_Node(cc);
    Model = glm::translate(glm::mat4(1), glm::vec3(0, 0, 128));
    back->relativeModel = Model;
    back->absoluteScale = glm::vec3(256, 256, 000);
    back->color = glm::vec4(21.f/255, 5.f/255, 42.f/255, 1.f);
    room->AddChild(back);
    walls.push_back(back);

    // Roof face:
    Scene_Node* roof = new Scene_Node(cc);
    Model = glm::translate(glm::mat4(1), glm::vec3(0, 256, 0));
    roof->relativeModel = Model;
    roof->absoluteScale = glm::vec3(256, 000, 256);
    roof->color = glm::vec4(5.f/255, 42.f/255, 40.f/255, 1.f);
    room->AddChild(roof);
    
    // Floor face:
    Scene_Node* floor = new Scene_Node(cc);
    floor->absoluteScale = glm::vec3(256, 000, 256);
    floor->color = glm::vec4(5.f/255, 42.f/255, 40.f/255, 1.f);
    room->AddChild(floor);

    // Door:
    door = new Scene_Node(cc);
    Model = glm::translate(glm::mat4(1), glm::vec3(128, 0, 0));
    door->relativeModel = Model;
    door->absoluteScale = glm::vec3(1, 128, 64);
    door->color = glm::vec4(1.f, 0.f, 0.f, 1.f);
    room->AddChild(door);
    
    // Orb:
    orb = new Scene_Node(cc);
    // Scene_Node* orb = new Scene_Node(cc);
    Model = glm::translate(glm::mat4(1), glm::vec3(0, 8, -64));
    Model = glm::rotate(Model, glm::radians(45.f), glm::vec3(0, 1, 0));
    Model = glm::rotate(Model, glm::radians(45.f), glm::vec3(1, 0, 0));
    orb->relativeModel = Model;
    orb->absoluteScale = glm::vec3(8);
    orb->color = glm::vec4(1, 0, 0, 1);
    room->AddChild(orb);

    // HUD:
    hud = new Scene_Node;
    hud->relativeModel = glm::translate(glm::mat4(1), glm::vec3(0, -0.9f, 0));

    Scene_Node* primaryColor = new Scene_Node(cc);
    Model = glm::translate(glm::mat4(1), glm::vec3(0.75f, 0, 0));
    primaryColor->relativeModel = Model;
    primaryColor->color = player->color;
    hud->AddChild(primaryColor);

    Scene_Node* secondaryColor = new Scene_Node(cc);
    Model = glm::translate(glm::mat4(1), glm::vec3(0.85f, 0, 0));
    secondaryColor->relativeModel = Model;
    hud->AddChild(secondaryColor);
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
    if (room)
        delete room;
    
    if (player)
        delete player;
    
    if (hud)
        delete hud;
}
