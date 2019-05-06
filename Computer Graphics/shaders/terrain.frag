#version 330

//in vec3 InterpolatedColor;
varying vec3 PixelPosition;
varying vec3 PixelNormal;

uniform vec2 iResolution;
uniform vec3 LightColor;
uniform vec3 LightPosition;
uniform vec3 CameraPosition;

out vec4 FragColor;

void main()
{
	vec3 Ambiental =  LightColor * .1;

	vec3 L = LightPosition - PixelPosition;
	L = normalize(L);
	float PX = dot(L, PixelNormal);
	PX = clamp(PX, 0., 1.);
	vec3 Difusa = PX * LightColor;

	vec3 R = reflect(-L, PixelNormal);
	vec3 V = CameraPosition - PixelPosition;
	V = normalize(V);
	float CX = clamp(dot(R, V), 0., 1.);
	CX = pow(CX, 64);
	vec3 Especular = CX * LightColor;

	vec3 phong = (Ambiental + Difusa + Especular);
	//* InterpolatedColor;

	FragColor = vec4(phong, 1.0);
}
