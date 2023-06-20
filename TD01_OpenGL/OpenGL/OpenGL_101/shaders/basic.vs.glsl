#version 120

attribute vec3 a_Position;
attribute vec3 a_Color;

varying vec4 v_Color;
uniform vec4 u_Color;

uniform mat4 view;
uniform mat4 projection;

void main(void)
{
    gl_Position = projection * view * vec4(a_Position, 1.0);
    v_Color = u_Color;
}