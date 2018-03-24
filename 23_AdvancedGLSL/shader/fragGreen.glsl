#version 330 core
out vec4 FragColor;

in VS_OUT // name should be same as vertex "VS_OUT"
{
    vec2 TexCoord;
	vec3 Position;
} fs_in;

void main()
{
    FragColor = vec4(0.0, 0.8, 0.0, 1.0);
}