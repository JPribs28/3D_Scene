#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform sampler2D ourTexture;

void main()
{
	// ambient
	float ambientStrength = 0.7f;
	vec3 ambient = ambientStrength * lightColor;

	// diffuse
	vec3 norm = normalize(normal);
	vec3 lightDirection = normalize(lightPos - fragPos);
	float diff = max(dot(norm, lightDirection), 0.0f);
	vec3 diffuse = diff * lightColor;

	//specular
	float specularStrength = 0.2f;
	vec3 viewDirection = normalize(viewPos - fragPos);
	vec3 reflectDirection = reflect(-lightDirection, norm);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0f), 8.0f);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * texture(ourTexture, texCoord).xyz;
	FragColor = vec4(result, 1.0f);
}