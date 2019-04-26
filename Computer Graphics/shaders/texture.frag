#version 330

in vec2 CoordsText;
out vec4 FragColor;

uniform sampler2D texturaCaja;

void main(){
	FragColor = texture2D(texturaCaja, CoordsText);
}