#version 450 core

in vec2 texCoord;
in vec3 fragPos;

out vec4 FragColor;

void main() {
   vec3 topColor = vec3(.337255f, .117647f, .058823f);
   vec3 bottomColor = vec3(.501961f, .254901f, .121569f);

   vec3 color = bottomColor * texCoord.y + topColor * (1.f - texCoord.y);

   FragColor = vec4(color, 1.f);
}