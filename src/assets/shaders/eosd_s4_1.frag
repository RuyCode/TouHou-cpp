#version 450 core

in vec2 texCoord;
in vec3 fragPos;

out vec4 FragColor;

void main() {
   FragColor = vec4(texCoord, 0.f, 1.f);
}