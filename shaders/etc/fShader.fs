#version 150
in vec4 color;
in vec2 texCoord;
in float diffuse;
    
in vec4 eye_pos;
in vec3 normal;
in vec3 light_direction0;
in vec3 light_direction1;
in vec3 light_direction2;

out vec4 fragColor;

void main()
{
	float diffuse = dot(normal, normalize(light_direction0));
	float ambient = 0.2;
	vec4 c = vec4(color.xyz * diffuse * (1.0 - ambient) + color.xyz * ambient, color.a);
	fragColor = clamp(c, 0.0, 1.0);
}