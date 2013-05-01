#version 120

attribute vec3 position;
attribute vec3 normal;
attribute vec2 texture_coord;

varying vec3 frag_normal;
varying vec2 frag_uv;

uniform mat4 mvp_matrix;
uniform mat4 mv_matrix;

void main()
{
	frag_normal = normalize(vec3(mv_matrix * vec4(normal, 0.0) ) );
	frag_uv = texture_coord;
	vec4 v_output = mvp_matrix * vec4(position, 1.0f);
	gl_Position = v_output;
}