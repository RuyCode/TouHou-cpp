#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec3 aTang;
layout (location = 3) in vec2 aTex;

out vec2 texCoord;
out vec3 fragPos;
out vec4 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 viewPos;

#pragma include "src/assets/shaders/PBR.vert"

void main() {
   texCoord = vec2(aTex.x, 1.0 - aTex.y);

   gl_Position = projection * view * model * vec4(aPos, 1.0);

   pos = gl_Position;

   PBRvert(model, aNorm, aTang);

   fragPos = vec3(model * vec4(aPos, 1.f));
}