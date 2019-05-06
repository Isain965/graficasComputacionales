#version 330

in vec3 VertexPosition;
in vec3 VertexNormal;


//out vec3 InterpolatedColor;
out vec3 PixelPosition;
out vec3 PixelNormal;

uniform mat4 modelMatrix;
uniform mat4 mvpMatrix;
uniform mat3 normalMatrix;

uniform sampler2D heightMap;
float alturaMax = 80.0f;
float width = 200.0f;
//float height = 200.0f;
vec3 new;

void main()
{
	new = VertexPosition;
	vec4 Textura = texture2D(heightMap, (new.xz*.501 + vec2(width*.5, width*.5))/width);
	PixelPosition = vec3(modelMatrix * vec4(new, 1.0f));
	PixelNormal = normalMatrix * VertexNormal;
	PixelNormal = normalize(PixelNormal);

	new.y =	Textura.x*alturaMax;
	gl_Position = mvpMatrix * vec4(new, 1.0f);
	//InterpolatedColor = VertexColor;
}