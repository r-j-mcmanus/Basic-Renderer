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
    v_FragPos = vec3(u_Model * a_Pos);
    gl_Position = lu_projection * lu_view *  vec4(v_FragPos, 1.0);
    v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal; 
};

#shader fragment
#version 330 core
out vec4 FragColor;

// inputs from v shader
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 v_Normal;
in vec3 v_FragPos;  

uniform Light u_light;
uniform Material u_material;
uniform vec3 u_viewPos;


void main()
{
    // ambient
    vec3 ambient = u_light.ambient * u_material.ambient;
  	
    // diffuse 
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_light.position - v_FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_light.diffuse * u_material.diffuse;

    // specular
    vec3 viewDir = normalize(u_viewPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
    vec3 specular = u_light.diffuse * (spec * u_material.specular); 

    // combine light types
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
};