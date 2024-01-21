attribute vec3 w4_a_normal;
attribute vec2 w4_a_uv0;

varying vec3 vN;
varying vec3 vPos;
varying vec3 vEye;
varying vec2 vUV;

void w4_main()
{
      vec4 pos = w4_getVertexPosition();
      vN = w4_u_normalSpace * w4_a_normal;
      vPos = pos.xyz;
      vEye = w4_u_eyePosition;
      vUV = w4_a_uv0;
      gl_Position = w4_u_projectionView * pos;
}