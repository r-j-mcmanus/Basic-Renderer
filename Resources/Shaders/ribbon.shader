#shader vertex
#version 330 core
layout(location = 0) in vec4 a_Pos;

layout (std140) uniform Matrices
{
    mat4 lu_projection;
    mat4 lu_view;
};
uniform mat4 u_Model;

void main()
{
    vec4 worldPos = u_Model * a_Pos;
    gl_Position = lu_projection * lu_view * worldPos;
};

#shader fragment
#version 330 core
out vec4 FragColor;


void main()
{   
    FragColor = vec4(153.0 / 255.0, 217.0/255.0, 234.0/255.0, 1.0);
};