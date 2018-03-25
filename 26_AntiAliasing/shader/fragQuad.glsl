#version 330 core
out vec4 FragColor;

in VS_OUT
{
    vec2 TexCoord;
} fs_in;

uniform sampler2D screenTexture;

void main()
{
	// Do some post-processing or not
	//vec3 tex = texture(screenTexture, fs_in.TexCoord).rgb;
    //float grayscale = 0.2126 * tex.r + 0.7152 * tex.g + 0.0722 * tex.b;
    //FragColor = vec4(vec3(grayscale), 1.0);
    FragColor = texture(screenTexture, fs_in.TexCoord);
}