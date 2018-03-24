#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out VS_OUT // interface block, name:VS_OUT
{
    vec2 TexCoord;
    vec3 Position;
} vs_out;

//layout(std140, binding = 2) uniform Matrices { ... }; // you can explicitly binding like this in version 420
layout (std140) uniform Matrices // uniform block with std140
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

void main()
{
    vs_out.TexCoord = aTexCoord;
    vs_out.Position = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}