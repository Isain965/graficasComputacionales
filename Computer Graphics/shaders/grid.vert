#version 330

uniform float time;

out vec4 InterpolatedColor;

void main() {

  float width = 10.0;
  
  float x = mod(gl_VertexID, width);
  float y = floor(gl_VertexID / width);
  
  float u = x / (width - 1.0f);
  float v = y / (width - 1.0f);
  
  float xOffset = cos(time + y * 0.2f) * 0.1f;
  float yOffset = cos(time + x * 0.3f) * 0.2f;
  
  float ux = u * 2.0f - 1.0f + xOffset;
  float uy = v * 2.0f - 1.0f + yOffset;
  
  vec2 xy = vec2(ux, uy) * 1.2f;
  
  gl_Position = vec4(xy, 0.0f, 1.0f);
  gl_PointSize = 10.0f;
  InterpolatedColor = vec4(cos(time * sin(y)), sin(time * cos(x)), 0.0f, 1.0f);
}