#version 110

uniform mat4 U_PROJECTION_TRANSFORM;
uniform mat4 U_VIEW_TRANSFORM;

attribute vec3 A_POSITION_0;
attribute vec3 A_NORMAL_0;
attribute vec2 A_TEX_COORD_0;

varying vec3 V_NORMAL_0;
varying vec2 V_TEX_COORD_0;

void main()
{
	V_NORMAL_0 = (U_VIEW_TRANSFORM * vec4(A_NORMAL_0, 0.0)).xyz;
	V_TEX_COORD_0 = A_TEX_COORD_0;
	gl_Position = U_PROJECTION_TRANSFORM * U_VIEW_TRANSFORM * vec4(A_POSITION_0, 1.0);
}
