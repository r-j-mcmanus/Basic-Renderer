#shader vertex
#version 330 core
layout(location = 0) in vec4 a_Pos;
layout (location = 1) in vec3 a_Normal;

out vec3 v_Normal;

uniform vec4 u_projection;
uniform vec4 u_view;


void main()
{
    gl_Position = u_projection * u_view * a_Pos;

    v_Normal =  a_Normal; 
};

#shader fragment
#version 330 core
out vec4 FragColor;

in vec3 v_Normal;
uniform vec4 u_Color;

void main()
{
    vec3 norm = normalize(v_Normal);
    FragColor = u_Color * norm[0];
};