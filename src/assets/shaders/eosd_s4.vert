#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec3 aTang;
layout (location = 3) in vec2 aTex;

out vec2 texCoord;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;
uniform vec3 viewPos;

#pragma include "src/assets/shaders/PBR.vert"

void main() {
   texCoord = aTex;
   
   gl_Position = projection * view * model * vec4(aPos, 1.0);

   PBRvert(model, aNorm, aTang);

   fragPos = vec3(model * vec4(aPos, 1.f));
}