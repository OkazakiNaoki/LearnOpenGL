#version 330 core
in vec2 texCoord;
in vec3 normal;
in vec3 pos;
out vec4 fragColor;

struct Material {
	sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct Light {
    vec3 direction; //replace position
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
  
uniform Material material;
uniform Light light;
uniform vec3 viewPos;
void main()
{
	vec3 ambient = light.ambient * texture(material.diffuse, texCoord).rgb;
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, texCoord).rgb;
	vec3 viewDir = normalize(viewPos - pos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, texCoord).rgb;
    fragColor = vec4(specular + diffuse + ambient, 1.0);
}