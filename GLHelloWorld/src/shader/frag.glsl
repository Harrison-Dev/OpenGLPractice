#version 330 core
out vec4 FragColor;

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

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float sine_time;
uniform float mix_t;

uniform vec3 lightColor;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;



void main()
{
	float sign_t = sign(sine_time);
    vec4 texColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(sign_t*TexCoord.x,TexCoord.y)), mix_t);

    // ambient
    vec3 ambient = light.ambient * material.ambient;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

	vec3 result = (ambient + diffuse + specular) /* * texColor.rgb */;
	FragColor = vec4(result, 1.0);
}