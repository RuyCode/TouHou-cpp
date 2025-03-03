#ifndef PBR_VERT
#define PBR_VERT

out mat3 TBN;

void PBRvert(mat4 model, vec3 norm, vec3 tang) {
    vec3 T = normalize(mat3(model) * tang);
    vec3 N = normalize(mat3(model) * norm);
    vec3 B = cross(N, T);

    TBN = mat3(T, B, N);
}

#endif