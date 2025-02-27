#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec3 aTang;
layout (location = 3) in vec2 aTex;

out vec2 texCoord;
out mat3 TBN;
out vec3 fragPos;

out float height;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main() {
   texCoord = aTex;

   gl_Position = projection * view * model * vec4(aPos, 1.0);

   vec3 T = normalize(mat3(model) * aTang);
   vec3 N = normalize(mat3(model) * aNorm);
   vec3 B = cross(N, T);

   TBN = mat3(T, B, N);

   fragPos = vec3(model * vec4(aPos, 1.f));
}