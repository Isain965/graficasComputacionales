#version 330

out vec4 InterpolatedColor;

uniform float time;

// Este metodo esta encargado de crear los circulos
vec2 circleCreator(float gl_VertexID) {
  float x = floor(gl_VertexID / 2.0);
  float y = mod(gl_VertexID + 1.0f, 2.0f);
  
  // Calcular el angulo, el 30 es el numero de triangulos que van a formar el circulo
  float angle = x / 30.0f * radians(360.0f);
							
  float radius = 4.0f - y;
  
  float u = radius * cos(angle);
  float v = radius * sin(angle);
  
  vec2 xy = vec2(u,v) * 0.2f;
  
  return xy;  
}


void main() { 

  float circle = floor(gl_VertexID / (20.0f * 6.0f));

  // Numero de circulos
  float numCirculos = 100;
  
  //Separacion de los circulos
  float space = floor(sqrt(numCirculos) * 0.50f);


  float width = 11.5f;
  
  float x = mod(circle, width);
  float y = floor(circle / width);
  
  float u = x / (width - 1.0f);
  float v = y / (width - 1.0f);
  
  float xOffset = cos(time + y * 0.2f) * 0.1f;
  float yOffset = cos(time + x * 0.3f) * 0.2f;
  
  float ux = u * 2.0f - 1.0f + xOffset;
  float vy = v * 2.0f - 1.0f + yOffset;
  
  vec2 xy = circleCreator(gl_VertexID) * 0.1f + vec2(ux,vy);
  
  gl_Position = vec4(xy , 0.0f, 1.0f);
  gl_PointSize = 10.0f;
  InterpolatedColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}