#version 330
layout(location=0) in vec3 position;

out vec4 v_color;
uniform mat4 M;

void main(){
    // Dividing by the aspect ratio to get squares:
    vec2 v_position = position.xy / vec2(16.f, 9.f);
    gl_Position = M * vec4(v_position, 0.0f, 1.0f);
    v_color = vec4(1.f, 1.f, 1.f, 1.f);
}
