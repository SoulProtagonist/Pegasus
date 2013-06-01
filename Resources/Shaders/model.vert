#version 120

attribute vec3 position;
attribute vec3 normal;
attribute vec2 texture_coord;
attribute vec3 tangent;
attribute vec3 bitangent;

varying vec2 frag_uv;
varying vec3 light_vec;
varying vec3 half_vec;
varying vec3 eye_vec;

uniform mat4 mvp_matrix;
uniform mat4 mv_matrix;
uniform mat3 n_matrix;

void main()
{
	vec3 lightPos = vec3(50.0, 50.0, 50.0);

	vec3 n = normalize(n_matrix * normal);
	vec3 t = normalize(n_matrix * tangent);
	vec3 b = normalize(n_matrix * bitangent);
	
	vec3 vertexPosition = vec3(mv_matrix * vec4(position, 1.0));
	vec3 lightDir = normalize(lightPos - vertexPosition);

	vec3 v;
	v.x = dot(lightDir, t);
	v.y = dot(lightDir, b);
	v.z = dot(lightDir, n);
	light_vec = normalize(v);

	v.x = dot(vertexPosition, t);
	v.y = dot(vertexPosition, b);
	v.z = dot(vertexPosition, n);
	eye_vec = normalize(v);

	vertexPosition = normalize(vertexPosition);
	vec3 hvec = normalize(vertexPosition + lightDir);
	v.x = dot(hvec, t);
	v.y = dot(hvec, b);
	v.z = dot(hvec, n);
	half_vec = v;

	frag_uv = texture_coord;
	vec4 v_output = mvp_matrix * vec4(position, 1.0f);
	gl_Position = v_output;
}