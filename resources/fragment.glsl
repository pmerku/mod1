#version 330 core
flat in vec3 flatColor;
in vec3 Color;
out vec4 FragColor;

void main() {
    FragColor = vec4(flatColor, 1.0);
}