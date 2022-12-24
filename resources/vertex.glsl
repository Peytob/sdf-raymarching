#version 330 core

layout (location = 0) in vec2 l_position;

void main() {
    gl_Position = vec4(l_position, 0.0, 1.0);
}
