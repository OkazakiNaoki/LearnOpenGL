#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Position;

uniform float fragCoordX;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{             
    if(gl_FragCoord.x < fragCoordX)
        FragColor = texture(texture1, TexCoord);
    else{
    	if(gl_FrontFacing)
        	FragColor = texture(texture1, TexCoord);
        else
        	FragColor = texture(texture2, TexCoord);
    }
}