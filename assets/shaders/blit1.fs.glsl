#version 410

uniform sampler2D U_SAMPLER_0;
uniform vec3 U_LIGHT_DIR_0;

in vec3 V_NORMAL_0;
in vec2 V_TEX_COORD_0;

out vec4 _FragColor;

void main()
{
	vec3 normal = normalize(V_NORMAL_0);
	vec2 pos = (V_TEX_COORD_0.xy - vec2(0.5)) * 2.0;
	float len = length(pos);
	float mask = step(1.0, len);
	
	vec4 albedo = texture(U_SAMPLER_0, V_TEX_COORD_0);
	albedo.a *= mask;

	_FragColor
		= vec4(max(0.2, abs(dot(normal, U_LIGHT_DIR_0)))
		* albedo.rgb, albedo.a);
}
