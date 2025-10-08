#version 110

uniform sampler2D U_SAMPLER_0;

varying vec2 V_TEX_COORD_0;

void main()
{
	gl_FragColor = texture2D(U_SAMPLER_0, V_TEX_COORD_0);
}
