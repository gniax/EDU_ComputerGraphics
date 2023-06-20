// Vertex shader
#version 120

attribute vec3 a_Position;
attribute vec3 a_Normal;
attribute vec2 a_TexCoords;

uniform float u_Time; // maj par glUniform1f

uniform mat4 u_ViewMatrix; 
uniform mat4 u_ProjectionMatrix;

uniform vec3 u_ViewPosition;

varying vec2 v_TexCoords;
varying vec3 v_Normal;

varying vec3 v_ViewDirection;

uniform mat4 model;

void main(void)
{
	v_TexCoords = vec2(a_TexCoords.x, 1.0 - a_TexCoords.y);
	
	v_Normal = mat3(model) * a_Normal;

	vec4 position = vec4(a_Position, 1.0);

	vec4 world_position = model * position;

	v_ViewDirection = normalize(u_ViewPosition - world_position.xyz);

	gl_Position = u_ProjectionMatrix
				* u_ViewMatrix 
				* world_position;
}
