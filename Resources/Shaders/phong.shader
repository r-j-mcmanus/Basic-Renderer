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

#define NUM_LIGHTS 8

struct Light {
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

layout (std140) uniform Lights {
    Light lu_lights[NUM_LIGHTS];
};

in vec3 v_Normal;
in vec3 v_FragPos;  

uniform Material u_material;
uniform vec3 u_viewPos;


void main()
{
    vec3 result = vec3(0.0);

    for(int i=0; i<NUM_LIGHTS; i++) {
        Light light = lu_lights[i];

        // ambient
        vec3 ambient = vec3(light.ambient) * u_material.ambient;
  	
        // diffuse 
        vec3 norm = normalize(v_Normal);
        vec3 lightDir = normalize(vec3(light.position) - v_FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * vec3(light.diffuse) * u_material.diffuse;

        // specular
        vec3 viewDir = normalize(u_viewPos - v_FragPos);
        vec3 reflectDir = reflect(-lightDir, norm); 
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
        vec3 specular = vec3(light.specular) * (spec * u_material.specular); 

        // combine light types
        result += ambient + diffuse + specular;
    }
    FragColor = vec4(result, 1.0);
};