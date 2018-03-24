#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 normal;
    mat4 projection;
    mat4 view;
    mat4 model;
} gs_in[];

const float MAGNITUDE = 0.2;

void GenerateLine(int index)
{
    gl_Position = gl_in[index].gl_Position;
    EmitVertex();
    // for some reason, normal transform will not perform correctly during vertex shader stage, so we move the calculation to here
    mat4 normalMatrix = mat4(mat3(transpose(inverse(gs_in[index].model))));
    vec4 normalVec = normalize(gs_in[index].projection * gs_in[index].view * normalMatrix * vec4(gs_in[index].normal, 0.0f));
    gl_Position = gl_in[index].gl_Position + normalVec * MAGNITUDE;
    EmitVertex();
    EndPrimitive();
}

void main()
{
    GenerateLine(0); // first vertex normal
    GenerateLine(1); // second vertex normal
    GenerateLine(2); // third vertex normal
}