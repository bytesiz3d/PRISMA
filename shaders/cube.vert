#version 300 es
layout(location=0) in vec3 position;
layout(location=1) in vec4 color; // We added a new attribute color at the location after position
layout(location=2) in vec2 texcoord; // We have a new attribute "texcoord" that contains the texture coordinates of the vertex

out vec4 v_color;
out vec2 v_texcoord; // We will pass the texture coordinates to the fragment shader

uniform mat4 M;
uniform mat4 VP;

void main(){
    vec3 v_position = position + vec3(0.f, 0.5f, 0.f);
    gl_Position = VP * M * vec4(v_position, 1.0f); // Just multiply by this matrix and..... welcome to the 3rd Dimension
    v_color = color; // Pass the color to the fragment shader
    v_texcoord = texcoord; // pass the texture coordinates as is to the fragment shader
    // gl_PointSize = 8.0;
}
