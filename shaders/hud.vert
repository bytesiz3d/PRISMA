#version 300 es
layout(location=0) in vec3 position;
layout(location=1) in vec4 color;

out vec4 vertexColor;

uniform mat4 M;

void main(){
    // Dividing by the aspect ratio to get squares:
    vec2 v_position = position.xy / vec2(16.f, 9.f);
    gl_Position = M * vec4(v_position, 0.0f, 1.0f);
    vertexColor = color;
}
