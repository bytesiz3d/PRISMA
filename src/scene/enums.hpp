// Enumerating object type and mesh
enum OBJECT_TYPE
{
    OBJECT_DOOR,
    OBJECT_WALL,
    OBJECT_ORB,
    OBJECT_NULL = -1
};

NLOHMANN_JSON_SERIALIZE_ENUM(OBJECT_TYPE, {
        {OBJECT_NULL, nullptr},
        {OBJECT_DOOR, "door"},
        {OBJECT_WALL, "wall"},
        {OBJECT_ORB, "orb"},
    })

enum MESH_TYPE
{
    // Add objects here as needed
    MESH_CUBE,
    MESH_SPHERE,
    MESH_MODEL0,
    MESH_MODEL1,
    MESH_MODEL2,
    MESH_MODEL3,
    MESH_NULL = 0
};

NLOHMANN_JSON_SERIALIZE_ENUM(MESH_TYPE, {
        {MESH_NULL, nullptr},
        {MESH_CUBE, "cube"},
        {MESH_SPHERE, "sphere"},
        {MESH_MODEL0, "model0"},
        {MESH_MODEL1, "model1"},
        {MESH_MODEL2, "model2"},
        {MESH_MODEL3, "model3"}
    })

enum DRAW_MODE
{
    DRAW_TRIANGLES = GL_TRIANGLES,
    DRAW_LINES = GL_LINE_STRIP,
    DRAW_NULL = 0
};

NLOHMANN_JSON_SERIALIZE_ENUM(DRAW_MODE, {
        {DRAW_NULL, nullptr},
        {DRAW_TRIANGLES, "triangles"},
        {DRAW_LINES, "lines"},
    })

// TODO: Reference textures in json
enum MESH_TEXTURE
{
    MESH_TEXTURE0,
    MESH_TEXTURE1,
    MESH_TEXTURE_NULL = -1
};
