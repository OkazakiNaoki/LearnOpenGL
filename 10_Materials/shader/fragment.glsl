#version 330 core
in vec3 normal;
in vec3 pos;
out vec4 fragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
  
uniform Material material;
uniform Light light;
uniform vec3 viewPos;
void main()
{
	vec3 ambient = light.ambient * material.ambient;
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - pos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);
	vec3 viewDir = normalize(viewPos - pos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);
    fragColor = vec4(specular + diffuse + ambient, 1.0);
}