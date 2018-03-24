#version 330 core
out vec4 FragColor;

in VS_OUT // name should be same as vertex "VS_OUT"
{
    vec2 TexCoord;
	vec3 Position;
} fs_in;

uniform float fragCoordX;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{             
    if(gl_FragCoord.x < fragCoordX)
        FragColor = texture(texture1, fs_in.TexCoord);
    else{
    	if(gl_FrontFacing)
        	FragColor = texture(texture1, fs_in.TexCoord);
        else
        	FragColor = texture(texture2, fs_in.TexCoord);
    }
}