attribute vec3 w4_a_normal;
attribute vec2 w4_a_uv0;

varying vec2 vUV;

uniform float flip_u;

void w4_main()
{    
    vec2 uv = w4_a_uv0;
    if (flip_u > 0.0) uv.x = 1.0 - uv.x;

    vUV = uv;
    gl_Position = w4_u_projectionView * w4_getVertexPosition();
}
