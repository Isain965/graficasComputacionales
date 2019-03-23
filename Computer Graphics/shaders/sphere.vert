#version 330

out vec4 InterpolatedColor;

uniform vec2 iResolution;
uniform float time;

// Matriz de rotacion en X
mat4 rotationInX() {
  float s = sin(time);
  float c = cos(time);
  return mat4(
    1, 0, 0, 0,
    0, c, s, 0,
    0, -s, c, 0,
    0, 0, 0, 1);
}

// Matriz de rotacion en Y
mat4 rotationInY() {
  float s = sin(time);
  float c = cos(time);
  return mat4(
    c, 0, s, 0,
    0, 1, 0, 0,
    -s, 0, c, 0,
    0, 0, 0, 1);
}

void main() {

  float vertex = mod(gl_VertexID, 6.0f);

  float v = (gl_VertexID - vertex) / 6.0f;

  float a1 = mod(v, 50.0f);
  v = (v - a1) / 50.0f;
  float a2 = v - 8.0f;
  
  // Creacion de los puntos que formaran la esfera
  // 50 * 2 * PI
  // que son las 50 diviciones en las que estara
  // dividida la esfera
  a1 = a1 / 50.0f * 2.0f * 3.1416f;
  a2 = a2 / 50.0f * 2.0f * 3.1416f;
  
  // Vector con todos los puntos
  vec4 spherePositions = vec4(cos(a1) * cos(a2), sin(a2), sin(a1) * cos(a2), 1.0f);
  
  
  // Se multiplica el vector de puntos con una matriz de rotacion en el eje X
  spherePositions *= rotationInX();
  
  // Se multiplica el vector de puntos con una matriz de rotacion en el eje X
  spherePositions = spherePositions * rotationInY();
  spherePositions = spherePositions * rotationInY();

  
  gl_Position = vec4(spherePositions.x * iResolution.y / iResolution.x, spherePositions.y, spherePositions.z * 0.5f + 0.5f, 1.0f);
  gl_PointSize = 4.0f;
  InterpolatedColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}