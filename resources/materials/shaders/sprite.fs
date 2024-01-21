uniform sampler2D texture0;
uniform vec4 color;

varying vec2 vUV;

void w4_main()
{
    //vec4 pixel = texture2D(texture0, vUV);
    //pixel *= color;

    vec4 outColor = texture2D(texture0, vUV) * color;
    if (vUV.y > 0.9988) outColor.a = 0.0;
    if (outColor.a < 0.001) discard;
    gl_FragColor = outColor;
    //gl_FragColor = vec4(0.5, 0.5, 0.5, 0.5);
}
