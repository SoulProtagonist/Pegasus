#version 120

attribute vec3 position;
attribute vec2 texture_coord;

varying vec2 uv;

uniform mat4 mvp_matrix;

void main()
{
	uv = texture_coord;
	vec4 output = vec4(position, 1.0f) * mvp_matrix;
	gl_Position = output;
}