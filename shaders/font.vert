#version 330
layout(location=0) in vec3 position;
layout(location=2) in vec2 texcoord; // We have a new attribute "texcoord" that contains the texture coordinates of the vertex

out vec4 v_color;
out vec2 v_texcoord; // We will pass the texture coordinates to the fragment shader

uniform mat4 M;
// uniform mat4 VP;

void main(){
    gl_Position = M * vec4(position.xy, 0.0f, 1.0f);
    v_color = vec4(1.f, 1.f, 1.f, 1.f);
    v_texcoord = texcoord; // pass the texture coordinates as is to the fragment shader
}
