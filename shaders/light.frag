#version 300 es
precision highp float;

in vec2 v_texcoord;
in vec3 v_normal;
in vec3 v_view;
in vec3 v_world;

out vec4 color;

uniform vec4 tint;
uniform sampler2D texture_sampler;

struct Material 
{
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
    float shininess;
};
uniform Material material;

struct Light 
{
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
    vec3 direction;
    vec3 position;
    float attenuation;
};

#define NUM_ROOMS 16
#define NUM_LIGHT_SUM_PER_ROOM 2 
uniform Light lights[NUM_ROOMS * NUM_LIGHT_SUM_PER_ROOM];
uniform int LightsNum ;

float diffuse(vec3 n, vec3 l)
{
    //Diffuse (Lambert) term computation: reflected light = cosine the light incidence angle on the surface
    //max(0, ..) is used since light shouldn't be negative
    return max(0.0f, dot(n, l));
}

float specular(vec3 n, vec3 l, vec3 v, float shininess)
{
    //Phong Specular term computation
    return pow(max(0.0f, dot(v, reflect(-l, n))), shininess);
}

void main()
{
    vec3 n = normalize(v_normal);
    vec3 v = normalize(v_view);
    vec3 light_sum = vec3(0.f);

    for (int i = 0; i < LightsNum; i++)
    {
        // For directional light_sum, the light vector is the inverse of the light direction
        vec3 l = -lights[i].direction;

        vec3 current_light =
            material.ambient * lights[i].ambient +
            material.diffuse * lights[i].diffuse * diffuse(n, l) +
            material.specular * lights[i].specular * specular(n, l, v, material.shininess);

        // Point light things:
        l = lights[i].position - v_world; // Point light

        // Normalize light vector and store distance
        float d = length(l);
        l /= d;
        float attenuation = lights[i].attenuation * d * d;

        current_light = 
            material.diffuse * lights[i].diffuse * diffuse(n, l) +
            material.specular * lights[i].specular * specular(n, l, v, material.shininess);

        current_light /= attenuation;
        current_light += material.ambient * lights[i].ambient;

        light_sum += current_light;
    }

    color = vec4(light_sum, 1.f);
    vec4 t_color = tint;
    t_color = texture(texture_sampler, v_texcoord) * t_color;
    color = color * t_color;
}
