#version 110

uniform sampler2D U_SAMPLER_0;
uniform vec3 U_LIGHT_DIR_0;

varying vec3 V_NORMAL_0;
varying vec2 V_TEX_COORD_0;

void main()
{
	vec3 normal = normalize(V_NORMAL_0);
	vec4 albedo = texture2D(U_SAMPLER_0, V_TEX_COORD_0);

	gl_FragColor
		= max(0.1, dot(normal, U_LIGHT_DIR_0))
		* albedo;
}
