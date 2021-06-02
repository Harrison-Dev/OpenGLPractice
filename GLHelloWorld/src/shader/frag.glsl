#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float sine_time;
uniform float mix_t;

void main()
{
	float sign_t = sign(sine_time);
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(sign_t*TexCoord.x,TexCoord.y)), mix_t);
}