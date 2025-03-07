#version 450 core

#pragma include "src/assets/shaders/PBR.frag"

in vec4 pos;

out vec4 FragColor;

uniform sampler2D background;

void main() {
   vec3 color = PBRfrag();
   float distanceToCamera = length(fragPos - viewPos);

   float fogDistance = 3.f;

   vec3 ndc = pos.xyz / pos.w;
   vec2 bgUV = ndc.xy * 0.5 + 0.5;

   vec3 bgColor = texture(background, bgUV).rgb;

   color = mix(color, bgColor, clamp((distanceToCamera - fogDistance) / fogDistance, 0.f, 1.f));

   FragColor = vec4(color, 1.f);
}