#shader vertex
#version 330 core
layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;

out vec3 v_FragPos; // so we can use them in the frag shader
out vec3 v_Normal;

layout (std140) uniform Matrices
{
    mat4 lu_projection;
    mat4 lu_view;
};

uniform mat4 u_model;

void main()
{
    v_FragPos = vec3(u_model * vec4(a_Pos, 1.0));
    // correctly handle any non-uniform scaling of the model matrix
    v_Normal = mat3(transpose(inverse(u_model))) * a_Normal; 
    
    gl_Position = lu_projection * lu_view * vec4(v_FragPos, 1.0);
};

#shader fragment
#version 330 core
out vec4 FragColor;

uniform vec4 u_Color;

void main()
{
    FragColor = u_Color;
};