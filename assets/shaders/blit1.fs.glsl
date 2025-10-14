#version 110

// uniform sampler2D U_SAMPLER_0;
uniform vec3 U_LIGHT_DIR_0;

varying vec3 V_NORMAL_0;
varying vec2 V_TEX_COORD_0;


void main()
{
	vec3 normal = normalize(V_NORMAL_0);
	vec2 pos = (V_TEX_COORD_0.xy - vec2(0.5)) * 2.0;
	float len = length(pos);
	float mask = step(1.0, len);
	
	// float gradient = fwidth(mask);
	// float alpha = gradient == 0.0 ? mask : 0.5;
	// vec4 albedo = vec4(1.0, 1.0, 1.0, alpha);
	vec4 albedo = vec4(mask);
	
	

	gl_FragColor
		= vec4(max(0.2, dot(normal, U_LIGHT_DIR_0))
		* albedo.rgb, albedo.a);
}
