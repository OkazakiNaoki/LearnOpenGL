#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 lightPos;
    vec3 viewPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform bool useNormalMap;

void main()
{           
     // obtain normal from normal map in range [0,1]
    vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
   
    // get diffuse color
    vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;
    // ambient
    vec3 ambient = 0.1 * color;
    
    vec3 lightDir;
    float diff;
    vec3 diffuse;
    vec3 viewDir;
    if(useNormalMap){
        // diffuse
        lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
        diff = max(dot(lightDir, normal), 0.0);
        diffuse = diff * color;
        // specular
        viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    }
    else{
        // diffuse
        lightDir = normalize(fs_in.lightPos - fs_in.FragPos);
        diff = max(dot(lightDir, normal), 0.0);
        diffuse = diff * color;
        // specular
        viewDir = normalize(fs_in.viewPos - fs_in.FragPos);
    }
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}