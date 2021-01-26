#version 330
layout(location=0) in vec3 position;

out vec4 v_color; // Since vertex shaders do not draw, we need to pass the color data to the fragment shader

uniform mat4 M;
uniform mat4 VP;

void main(){
    gl_Position = VP * M * vec4(position, 1.0f); // Just multiply by this matrix and..... welcome to the 3rd Dimension
    v_color = vec4(1.f, 1.f, 1.f, 1.f); // Pass the color to the fragment shader
    // gl_PointSize = 8.0;
}
