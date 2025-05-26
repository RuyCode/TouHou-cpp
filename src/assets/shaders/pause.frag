uniform sampler2D frame;
uniform float time;

float hash(float n) {
    return fract(sin(n) * 43758.5453123);
}

float smoothNoise(float t) {
    float tInt = floor(t);
    float tFrac = fract(t);
    float a = hash(tInt);
    float b = hash(tInt + 1.0);
    return mix(a, b, smoothstep(0.0, 1.0, tFrac));
}

float getGlitchOffset() 
{

    float seed = floor(time * 1.5);
    float rnd = fract(sin(seed * 1234.567) * 43758.5453);
    float glitchActive = step(0.8, rnd);
    float glitchValue = rnd * 6.;

    float base = 50. * (smoothNoise(time * 2.0) - 0.5) * 0.1;

    return mix(base, glitchValue, glitchActive);
};

void main()
{
    vec2 offsetFactor = vec2(0.02);

    vec2 textureCoordinates = gl_TexCoord[0].xy;
    vec4 color = vec4(0.0);

    color += texture2D(frame, textureCoordinates - offsetFactor * 4.0) * 0.0162162162;
    color += texture2D(frame, textureCoordinates - offsetFactor * 3.0) * 0.0540540541;
    color += texture2D(frame, textureCoordinates - offsetFactor * 2.0) * 0.1216216216;
    color += texture2D(frame, textureCoordinates - offsetFactor) * 0.1945945946;
    color += texture2D(frame, textureCoordinates) * 0.2270270270;
    color += texture2D(frame, textureCoordinates + offsetFactor) * 0.1945945946;
    color += texture2D(frame, textureCoordinates + offsetFactor * 2.0) * 0.1216216216;
    color += texture2D(frame, textureCoordinates + offsetFactor * 3.0) * 0.0540540541;
    color += texture2D(frame, textureCoordinates + offsetFactor * 4.0) * 0.0162162162;

    color *= vec4(0.18, 0.36, 0.75, 1.0) * 2.;

    float aberrationStrength = getGlitchOffset();
    vec2 resolution = vec2(384.0, 448.0);

    vec2 offset = aberrationStrength / resolution;

    vec2 uv = gl_TexCoord[0].xy;

    vec4 colorR = texture2D(frame, uv + offset * vec2(1.0, 0.0));
    vec4 colorG = texture2D(frame, uv);
    vec4 colorB = texture2D(frame, uv - offset * vec2(1.0, 0.0));

    color = mix(vec4(colorR.r, colorG.g, colorB.b, 1.0), color, .5);

    gl_FragColor = color;
}