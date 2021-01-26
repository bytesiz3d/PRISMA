#version 330
precision highp float;

in vec4 v_color;
out vec4 color;

uniform vec4 tint;

void main(){
    color = tint * v_color; // Send our interpolated color
}
