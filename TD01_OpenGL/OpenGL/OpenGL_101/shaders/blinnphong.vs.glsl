#version 120

// role du Vertex Shader:
// produire (au minimum)
// une position

// les entrees (inputs):
// le mot cle 'attribute' 
// indique que la donnee
// provient du C++

attribute vec3 a_Position;
attribute vec3 a_Normal;
attribute vec2 a_TexCoords;

uniform float u_Time; // maj par glUniform1f

uniform mat4 u_WorldMatrix;
uniform mat4 u_ViewMatrix; // a faire !
uniform mat4 u_ProjectionMatrix;

uniform vec3 u_ViewPosition;

varying vec2 v_TexCoords;
varying vec3 v_Normal;

varying vec3 v_ViewDirection;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(void)
{
	v_TexCoords = vec2(a_TexCoords.x, 1.0 - a_TexCoords.y);
	
	v_Normal = mat3(u_WorldMatrix) * a_Normal;

	vec4 position = vec4(a_Position, 1.0);

	// la position de la camera est exprimee dans le repere du monde
	// il faut donc d'abord changer le repere de l'objet de local vers monde
	vec4 world_position = u_WorldMatrix * position;

	// viewDirection est un vecteur directeur, et doit se normaliser
	v_ViewDirection = normalize(u_ViewPosition - world_position.xyz);

	gl_Position = u_ProjectionMatrix
				/* u_ViewMatrix */
				* world_position;
}

// gl_Position est une var.
// predefinie de type vec4
// avec le mot cle 'varying'
