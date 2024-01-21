attribute vec3 w4_a_normal;
attribute vec2 w4_a_uv0;


void w4_main()
{
    gl_Position = w4_u_projectionView * w4_getVertexPosition();
}
