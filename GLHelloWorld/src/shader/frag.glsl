#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float sine_time;
uniform float mix_t;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
	float sign_t = sign(sine_time);
    vec4 texColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(sign_t*TexCoord.x,TexCoord.y)), mix_t);
	vec4 lightColor = vec4((lightColor * objectColor), 1.0);
	FragColor =  texColor * lightColor;
}