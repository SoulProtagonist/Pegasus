#version 120

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;

varying vec2 frag_uv;
varying vec3 light_vec;
varying vec3 eye_vec;
varying vec3 half_vec;

void main()
{
	vec3 normal = 2.0 * texture2D(normalMap, frag_uv).rgb - 1.0;
	normal = normalize(normal);
	float lamberFac = max( dot(light_vec, normal ), 0.0);
	
	vec4 diffuseColor = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 specColor = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 amb = vec4(0.0, 0.0, 0.0, 1.0);
	
	if(lamberFac > 0.0) 
	{
		float NdotHV = max( dot(normal, half_vec), 0.0);
		diffuseColor += texture2D(diffuseMap, frag_uv) * lamberFac;
		specColor += texture2D(specularMap, frag_uv) * pow(NdotHV, 128);	
	}

	gl_FragColor = diffuseColor + specColor + amb;
}