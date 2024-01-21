uniform sampler2D texture0;
uniform float alpha;
uniform float tileMultipleX;
uniform float tileMultipleY;

varying vec2 vUV;

void w4_main()
{
    //vec4 outColor = texture2D(texture0, clamp(vUV, 0.0, 1.0));
    vec4 outColor = texture2D(texture0, vUV * vec2(tileMultipleX, tileMultipleY));
    outColor.a *= alpha;
    if (vUV.y > 0.9988) outColor.a = 0.0;
    if (outColor.a < 0.001) discard;

    gl_FragColor = outColor;
    //gl_FragColor = vec4(0.5, 0.5, 0.5, 0.5);
}
