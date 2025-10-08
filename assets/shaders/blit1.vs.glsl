#version 110

uniform mat4 U_PROJECTION_TRANSFORM;

attribute vec4 A_POSITION_0;

varying vec2 V_TEX_COORD_0;

void main()
{
	V_TEX_COORD_0 = A_POSITION_0.xy;
	
	gl_Position = U_PROJECTION_TRANSFORM * A_POSITION_0;
}
