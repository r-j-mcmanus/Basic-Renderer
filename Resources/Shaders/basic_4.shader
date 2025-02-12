#shader vertex
#version 330 core
layout(location = 0) in vec4 a_Pos;
layout (location = 1) in vec3 a_Normal;

out vec3 v_Normal;
out vec3 v_FragPos;

layout (std140) uniform Matrices
{
    mat4 lu_projection;
    mat4 lu_view;
};
uniform mat4 u_Model;

void main()
{
    gl_Position = lu_projection * lu_view * u_Model * a_Pos;
    v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal; 
    v_FragPos = vec3(u_Model * a_Pos);
};

#shader fragment
#version 330 core
out vec4 FragColor;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 v_Normal;
in vec3 v_FragPos;  

uniform vec3 u_Color;

uniform Light u_light;


void main()
{
    // ambient
    vec3 ambient = u_light.ambient * u_Color;
  	
    // diffuse 
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_light.position - v_FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = u_light.diffuse * diff;

    // combine light types
    vec3 result = (ambient + diffuse);
    FragColor = vec4(result, 1.0);
};