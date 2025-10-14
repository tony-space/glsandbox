#version 410

uniform mat4 U_PROJECTION_TRANSFORM;
uniform mat4 U_VIEW_TRANSFORM;

in vec3 A_POSITION_0;
in vec3 A_NORMAL_0;
in vec2 A_TEX_COORD_0;

out vec3 V_NORMAL_0;
out vec2 V_TEX_COORD_0;

void main()
{
	V_NORMAL_0 = (U_VIEW_TRANSFORM * vec4(A_NORMAL_0, 0.0)).xyz;
	V_TEX_COORD_0 = A_TEX_COORD_0;
	gl_Position = U_PROJECTION_TRANSFORM * U_VIEW_TRANSFORM * vec4(A_POSITION_0, 1.0);
}
