#version 450 core

in vec2 texCoord;
in mat3 TBN;
in vec3 fragPos;
in float height;

uniform sampler2D material_albedo_1;
uniform sampler2D material_metallic_1;
uniform sampler2D material_normal_1;
uniform sampler2D material_roughness_1;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform vec4 clearColor;

out vec4 FragColor;


const float PI = 3.14159265359;

vec3 FresnelSchlick(float cosTheta, vec3 F0) {
   return F0 + (1.f - F0) * pow(1.f - cosTheta, 5.f); 
}

float DistributionGGX(vec3 N, vec3 H, float roughness) {
   float a = roughness * roughness;
   float a2 = a * a;
   float NdotH = max(dot(N, H), 0.f);
   float NdotH2 = NdotH * NdotH;

   float num = a2;
   float denom = (NdotH2 * (a2 - 1.f) + 1.f);
   denom = PI * denom * denom;\

   return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
   float r = (roughness + 1.f);
   float k = (r * r) / 8.f;

   float num = NdotV;
   float denom = NdotV * (1.f - k) + k;

   return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
   float NdotV = max(dot(N, V), 0.f);
   float NdotL = max(dot(N, L), 0.f);
   float ggx2 = GeometrySchlickGGX(NdotV, roughness);
   float ggx1 = GeometrySchlickGGX(NdotL, roughness);

   return ggx1 * ggx2;
}

void main() {
   vec3 N =  texture(material_normal_1, texCoord).rgb;
   N = N * 2.f - 1.f;
   N = normalize(TBN * N);

   vec3 V = normalize(viewPos - fragPos);

   vec3 F0 = vec3(.04f);
   float metallic = texture(material_metallic_1, texCoord).b;
   vec3 albedo = texture(material_albedo_1, texCoord).rgb;
   F0 = mix(F0, albedo, metallic);

   vec3 Lo = vec3(0.f);

   // add lighting loop for more than 1 light source. lightPos[i] and lightColor[i]
   vec3 L = normalize(lightPos - fragPos);
   vec3 H = normalize(V + L);

   float distance = length(lightPos - fragPos);
   float attenuation = 1.f / (distance * distance);
   vec3 radiance = lightColor * attenuation;

   float roughness = texture(material_roughness_1, texCoord).g;
   float NDF = DistributionGGX(N, H, roughness);
   float G = GeometrySmith(N, V, L, roughness);
   vec3 F = FresnelSchlick(max(dot(H, V), 0.f), F0);

   vec3 kS = F;
   vec3 kD = vec3(1.f) - kS;
   kD *= 1.f - metallic;

   vec3 numerator = NDF * G * F;
   float denominator = 4.0 * max(dot(N, V), 0.f) * max(dot(N, L), 0.f);
   vec3 specular = numerator / max(denominator, .001f);
   

   float NdotL = max(dot(N, L), 0.f);
   Lo += (kD * albedo / PI + specular) * radiance * NdotL;

   // end lighting loop

   vec3 ambient = vec3(.03f) * albedo;
   vec3 color = ambient + Lo; 

   color = color / (color + vec3(1.f));
   color = pow(color, vec3(1.f / 2.2f));

   float distanceToCamera = length(fragPos - viewPos);

   float fogDistance = 2.3f;

   color = mix(color, clearColor.rgb, clamp((distanceToCamera - fogDistance) / fogDistance, 0.f, 1.f));

   FragColor = vec4(color, 1.f);
}