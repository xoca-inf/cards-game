uniform sampler2D texture0;
uniform float tileMultipleY;

varying vec3 vN;
varying vec3 vPos;
varying vec2 vUV;
varying vec3 vEye;

uniform vec3 light_params; // .x - diffuse Intensity, .y - specular Intensity, .z - ambient Intensity
uniform float shinessParam;

void w4_userLightModel(inout vec3 results[2], vec3 vectorToLight, vec3 vectorToEye, vec3 N, vec3 lColor, float shadowFactor, float fading)
{
    w4_BlinnLightModel(results, vectorToLight, vectorToEye, N, lColor, shadowFactor, fading, shinessParam);
}

void w4_main()
{
    vec4 diffuseTexture = texture2D(texture0, vUV * vec2(1.0, tileMultipleY) );

    vec3 lightFactors[2];
    w4_calculateLightFactor(lightFactors, vPos, vEye, normalize(vN));

    vec3 diffuse  = lightFactors[0] * light_params.x;
    vec3 specular = lightFactors[1] * light_params.y;

    vec3 resultColor = diffuseTexture.rgb * (diffuse + specular + light_params.zzz);

    gl_FragColor = vec4(resultColor, 1.0);
}
