#shader vertex
#version 330 core
layout(location = 0) in vec4 aPos;
layout (location = 1) in vec3 a_Normal;

out vec3 v_Normal;

layout (std140) uniform Matrices
{
    mat4 lu_projection;
    mat4 lu_view;
};

uniform mat4 u_Model;

void main()
{
    gl_Position = lu_projection * lu_view * u_Model * aPos;
    v_Normal =  a_Normal; 
}


#shader fragment
#version 330 core
in vec3 v_Normal;

out vec4 FragColor;

void main()
{    
    vec4 color = vec4(1.0, 0.5, 0.2, 1.0); // Orange color
    vec3 norm = normalize(v_Normal);
    FragColor = color * norm[0];
};