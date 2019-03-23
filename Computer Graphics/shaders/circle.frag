#version 330


out vec4 FragColor;

uniform vec2 iResolution;

// Funcion para calcular los colores en RGB
vec3 RGB(vec2 uv, float s) {
    float a = acos(uv.y / length(uv)) / (3.1416f * 2.0f);
    if(uv.x < 0.) a = 1.-a;

    a = ceil(a*s-.5) / s;
    return clamp(abs(abs(vec3(a , a - 1.0 / 3.0f, a - 2.0f / 3.0f )) - 0.5f) * 4.0f - 1.0f, 0.0f, 1.0f);
}

void main() {
	// Posicion del centro
	vec2 centro =  vec2(0.5f, 0.5f);
	centro = centro - (gl_FragCoord.xy / iResolution);

	// Aqui estamos asignando la concentracion de colores 
	// que se van a pintar a los vertices
    vec2 uv = gl_FragCoord.xy - iResolution.xy * 0.5f;

	// Aqui estamos quitando la parte del centro al circulo
	// Si la distancia al centro de la ventana es menor que 0.25
	// entonces descarta
	if(length(centro) < 0.25){
		gl_FragColor = vec4(1.0f, 1.0f, 0.5f, 1.0f);
	} else {
        gl_FragColor = vec4(RGB(uv, 360.0f), 1.0f);
	}
}
