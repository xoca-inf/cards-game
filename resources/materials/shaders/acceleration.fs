uniform sampler2D texture0;
uniform sampler2D texture1;
uniform vec4 color;
uniform float percent;

varying vec2 vUV;

void w4_main()
{
    vec4 ful = texture2D(texture0, vUV) * color;
    vec4 unful = texture2D(texture1, vUV) * color;
    
    if (vUV.y >= percent)
    {
        gl_FragColor = unful;
    }
    else
    {
        gl_FragColor = ful;
    }
}
