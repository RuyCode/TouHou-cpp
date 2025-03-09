#ifndef PBR_FRAG
#define PBR_FRAG

in vec2 texCoord;
in mat3 TBN;
in vec3 fragPos;

uniform sampler2D material_albedo_1;
uniform sampler2D material_metallic_1;
uniform sampler2D material_normal_1;
uniform sampler2D material_roughness_1;

const int bufferSize = 64;

uniform int lightNumber;
uniform vec3 lightPositions[bufferSize];
uniform vec3 lightColors[bufferSize];


uniform vec3 viewPos;

const float PI = 3.14159265359f;

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

vec3 PBRfrag() {
    // Sample material properties
    vec3 albedo = texture(material_albedo_1, texCoord).rgb;
    float metallic = texture(material_metallic_1, texCoord).r;
    float roughness = texture(material_roughness_1, texCoord).r;
    vec3 normal = texture(material_normal_1, texCoord).rgb * 2.0 - 1.0;
    normal = normalize(TBN * normal);

    // Compute view direction
    vec3 V = normalize(viewPos - fragPos);

    // Compute reflectance at normal incidence (F0)
    vec3 F0 = mix(vec3(0.04), albedo, metallic);

    vec3 Lo = vec3(0.0);
    for (int i = 0; i < lightNumber; i++) {
        vec3 L = normalize(lightPositions[i] - fragPos);
        vec3 H = normalize(V + L);
        float distance = length(lightPositions[i] - fragPos);
        float attenuation = 1.0 / (distance * distance + 0.1); // Prevent extreme falloff
        vec3 radiance = lightColors[i] * attenuation;

        // Cook-Torrance BRDF calculations
        float NDF = pow(max(dot(normal, H), 0.0), (roughness * roughness * 128.0));
        float k = (roughness + 1.0) * (roughness + 1.0) / 8.0;
        float G = (dot(normal, V) / (dot(normal, V) * (1.0 - k) + k)) * 
                  (dot(normal, L) / (dot(normal, L) * (1.0 - k) + k));
        vec3 F = F0 + (1.0 - F0) * pow(1.0 - max(dot(H, V), 0.0), 5.0);

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(normal, V), 0.0) * max(dot(normal, L), 0.0) + 0.001;
        vec3 specular = numerator / denominator;

        // Diffuse reflection
        vec3 kD = (1.0 - F) * (1.0 - metallic);
        vec3 diffuse = kD * albedo;

        // Accumulate contribution
        float NdotL = max(dot(normal, L), 0.0);
        Lo += (diffuse + specular) * radiance * NdotL;
    }

    // Ambient lighting (boosted for visibility)
    vec3 ambient = vec3(0.3) * albedo; // Increased ambient multiplier

    return ambient + Lo;
}


#endif