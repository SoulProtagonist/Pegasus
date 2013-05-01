#version 120

uniform sampler2D diffuse;

varying vec3 frag_normal;
varying vec2 frag_uv;

void main()
{
	vec3 lightDir = normalize( vec3(1.0, 1.0, 1.0) );
	vec3 n = normalize( frag_normal );
	float intensity = max( dot(lightDir, n ), 0.0);
	vec4 color = texture2D(diffuse, frag_uv);
	vec4 amb = color * 0.33;

	gl_FragColor = (color * intensity) + amb;
}