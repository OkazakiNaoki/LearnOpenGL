#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 instanceMatrix; // method 2!

out vec2 texCoord;

uniform mat4 projection;
uniform mat4 view;
//uniform mat4 model;

void main()
{
    //gl_Position = projection * view * model * vec4(aPos, 1.0);
    gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0);
    texCoord = aTexCoord;
}