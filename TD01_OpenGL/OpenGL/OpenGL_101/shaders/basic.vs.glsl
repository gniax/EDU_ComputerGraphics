#version 120

// role du Vertex Shader:
// produire (au minimum)
// une position

// les entrees (inputs):
// le mot cle 'attribute' 
// indique que la donnee
// provient du C++

attribute vec2 a_Position;

attribute vec3 a_Color;

varying vec4 v_Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(void)
{
	v_Color = vec4(
	    a_Color, 1.0);


	gl_Position = 
	vec4(a_Position.xy, 0.0, 1.0);
}

// gl_Position est une var.
// predefinie de type vec4
// avec le mot cle 'varying'
