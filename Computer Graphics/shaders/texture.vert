#version 330

in vec3 VertexPosition;
in vec2 VertCoord;

uniform mat4 mvp;

out vec2 CoordsText;

void main()
{
  gl_Position = mvp * vec4(VertexPosition,1.0f);
  CoordsText = VertCoord;
}