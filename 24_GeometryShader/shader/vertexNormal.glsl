#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out VS_OUT {
    vec3 normal;
	mat4 projection;
	mat4 view;
	mat4 model;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    vs_out.normal = aNormal;
    vs_out.projection = projection;
    vs_out.view = view;
    vs_out.model = model;
    gl_Position = projection * view * model * vec4(aPos, 1.0); 
}