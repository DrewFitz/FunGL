#version 150

in vec4 color_[];
in vec2 texCoord_[];

in vec4 eye_pos_[];
in vec3 normal_[];
in vec3 light_direction0_[];
in vec3 light_direction1_[];
in vec3 light_direction2_[];

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

out vec4 color;
out vec2 texCoord;

out vec4 eye_pos;
out vec3 normal;
out vec3 light_direction0;
out vec3 light_direction1;
out vec3 light_direction2;

void main() {
    for(int i = 0; i < gl_in.length(); i++) {
        
		color=color_[i];
        texCoord=texCoord_[i];
		eye_pos=eye_pos_[i];
		normal=normal_[i];
		light_direction0=light_direction0_[i];
		light_direction1=light_direction1_[i];
		light_direction2=light_direction2_[i];
		
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}
