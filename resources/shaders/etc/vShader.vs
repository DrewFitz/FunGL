#version 150
in vec3 in_position;
in vec3 in_normal;
in vec4 in_color;
in vec2 in_texCoord;

uniform bool useModelColors = false;

uniform mat4 model		= mat4(vec4(1.0, 0.0, 0.0, 0.0),
							   vec4(0.0, 1.0, 0.0, 0.0),
							   vec4(0.0, 0.0, 1.0, 0.0),
							   vec4(0.0, 0.0, 0.0, 1.0));
uniform mat4 view		= mat4(vec4(1.0, 0.0, 0.0, 0.0),
							   vec4(0.0, 1.0, 0.0, 0.0),
							   vec4(0.0, 0.0, 1.0, 0.0),
							   vec4(0.0, 0.0, 0.0, 1.0));
uniform mat4 projection	= mat4(vec4(1.0, 0.0, 0.0, 0.0),
							   vec4(0.0, 1.0, 0.0, 0.0),
							   vec4(0.0, 0.0, 1.0, 0.0),
							   vec4(0.0, 0.0, 0.0, 1.0));

uniform mat4 modelView;

uniform vec3 lightSource0 = vec3(0.0, 1.0, -3.0);
uniform vec3 lightSource1 = vec3(4.0, 3.0, -1.0);
uniform vec3 lightSource2 = vec3(-5.0, -1.0, -1.0);

out vec4 color_;
out vec2 texCoord_;

out vec4 eye_pos_;
out vec3 normal_;
out vec3 light_direction0_;
out vec3 light_direction1_;
out vec3 light_direction2_;

void main() {
    texCoord_ = in_texCoord;
    color_ = useModelColors ? in_color : vec4(0.66, 0.66, 0.66, 0.8);
	
	//eye_pos_ = view * model * vec4(in_position, 1.0);
	eye_pos_ = modelView * vec4(in_position, 1.0);
	
	vec4 cheat_normal_;
	//normal_ = normalize( (transpose(inverse(view * model)) * vec4(in_normal,0.0)).xyz );
	//cheat_normal_ = view * model * vec4(in_normal, 0.0);
	cheat_normal_ = modelView * vec4(in_normal, 0.0);
	normal_ = normalize(cheat_normal_.xyz);
	
	light_direction0_ = normalize(lightSource0 - eye_pos_.xyz);
	light_direction1_ = normalize(lightSource1 - eye_pos_.xyz);
	light_direction2_ = normalize(lightSource2 - eye_pos_.xyz);
	    	
    gl_Position = projection * eye_pos_; 
}