#version 330 core
out vec4 FragColor;

in VS_OUT
{
	vec3 Position;
	vec3 Normal;
    vec2 TexCoord;
} fs_in;

uniform sampler2D floorTexture;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool blinn;

void main()
{
	float gamma = 2.2;
    //vec3 color = pow(texture(floorTexture, fs_in.TexCoord).rgb, vec3(gamma)); // if texture already is sRGB space, correction method 1
    vec3 color = texture(floorTexture, fs_in.TexCoord).rgb; // texture correction method 2, glTexImage2D's format argument set as GL_SRGB/GL_SRGB_ALPHA
    // ambient
    vec3 ambient = 0.05 * color;
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.Position);
    vec3 normal = normalize(fs_in.Normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.Position);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    if(blinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), 32);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8);
    }
    vec3 specular = vec3(0.3) * spec;

    float distance = length(lightPos - fs_in.Position);
    float attenuation = 1.0 / (distance * distance);
    diffuse *= attenuation;
    specular *= attenuation;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma)); // Gamma correction option#2 : do it in fragment shader
}