#version 330 core
in vec3 normal;
in vec3 pos;
out vec4 fragColor;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform float ambientStrength;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float specularStrength;
void main()
{
	vec3 ambient = ambientStrength * lightColor;
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - pos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	vec3 viewDir = normalize(viewPos - pos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor; 
	//fragColor = vec4(ambient * objectColor, 1.0);
    //fragColor = vec4((diffuse + ambient) * objectColor, 1.0);
    fragColor = vec4((specular + diffuse + ambient) * objectColor, 1.0);
}