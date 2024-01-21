uniform sampler2D texture0;
uniform vec4 color;
uniform float percent;

varying vec2 vUV;

void w4_main()
{
    vec4 unful = texture2D(texture0, vUV) * color;
    vec4 ful = vec4(0.0, 1.0, 0.0, unful.w);
    if (vUV.x > percent)
    {
        gl_FragColor = unful;
    }
    else
    {
        gl_FragColor = ful;
    }
}
