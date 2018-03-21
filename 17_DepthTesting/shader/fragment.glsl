#version 330 core
in vec2 texCoord;
out vec4 fragColor;

uniform sampler2D texture_diffuse1;

float near = 0.1; 
float far  = 50.0;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC(Normalized Device Coordinates)
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
	float depth = LinearizeDepth(gl_FragCoord.z) / far;
	fragColor = vec4(vec3(depth), 1.0);
}