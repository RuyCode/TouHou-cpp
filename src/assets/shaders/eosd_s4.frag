#version 450 core

#pragma include "src/assets/shaders/PBR.frag"

uniform vec4 clearColor;

void main() {
   vec3 color = PBRfrag();
   float distanceToCamera = length(fragPos - viewPos);

   float fogDistance = 8.f;

   color = mix(color, clearColor.rgb, clamp((distanceToCamera * distanceToCamera - fogDistance) / fogDistance, 0.f, 1.f));

   FragColor = vec4(color, 1.f);
}