#version 330 core
out vec3 FragColor;

in vec2 TexCoords;

uniform sampler2D ssaoInput;

void main() 
{
    vec2 texelSize = vec2(textureSize(ssaoInput, 0));
    float result = 0.0;
    for (int x = -2; x < 2; ++x) 
    {
        for (int y = -2; y < 2; ++y) 
        {
            vec2 offset = vec2(float(x), float(y)) / texelSize;
            result += texture(ssaoInput, TexCoords + offset).r;
        }
    }
    result = result / (4.0 * 4.0);
    FragColor = vec3(result);
}  