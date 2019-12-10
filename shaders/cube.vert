#version 300 es
layout(location=0) in vec3 position;
layout(location=1) in vec4 color; // We added a new attribute color at the location after position

out vec4 vertexColor; // Since vertex shaders do not draw, we need to pass the color data to the fragment shader

uniform mat4 M;
uniform mat4 VP;

void main(){
    vec3 v_position = position + vec3(0.f, 0.5f, 0.f);
    gl_Position = VP * M * vec4(v_position, 1.0f); // Just multiply by this matrix and..... welcome to the 3rd Dimension
    vertexColor = color; // Pass the color to the fragment shader
    // gl_PointSize = 8.0;
}
