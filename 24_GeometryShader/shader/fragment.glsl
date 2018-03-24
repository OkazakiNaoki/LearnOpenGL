#version 330 core
out vec4 FragColor;

//in vec2 TexCoord;
in VS_OUT {
    vec2 texCoord;
} fs_in;

uniform sampler2D texture1;

void main()
{
    //FragColor = texture(texture1, TexCoord);
    FragColor = texture(texture1, fs_in.texCoord);
}