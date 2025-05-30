#version 330
layout(location=0) in vec3 position;
layout(location=3) in vec4 color;

out vec4 v_color;

uniform mat4 M;

void main() {
    gl_Position = M * vec4(position.xy, 0.9f, 1.0f);
    v_color = color; // pass the color to the fragment shader
}
