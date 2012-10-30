#version 150

in vec4 color_[];
in vec2 texCoord_[];

in vec4 eye_pos_[];
in vec3 normal_[];
in vec3 light_direction0_[];
in vec3 light_direction1_[];
in vec3 light_direction2_[];

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

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
				
		vec4 p0,p1,p2,p3;
		vec3 o0, o1, o2, o3;
		vec3 p = normalize(gl_in[i].gl_Position.xyz);
		
		float sizeFactor = 0.03;
		
		o0 = normalize(cross(normal, p));
		o1 = normalize(cross(normal, o0));
		o2 = -o1;
		o3 = -o0;
		
		p0 = gl_in[i].gl_Position + vec4(o0, 0.0) * sizeFactor;
		p1 = gl_in[i].gl_Position + vec4(o1, 0.0) * sizeFactor;
		p2 = gl_in[i].gl_Position + vec4(o2, 0.0) * sizeFactor;
		p3 = gl_in[i].gl_Position + vec4(o3, 0.0) * sizeFactor;
		
		gl_Position = p0;
        EmitVertex();
		
		gl_Position = p1;
        EmitVertex();
		
		gl_Position = p2;
        EmitVertex();
		
		gl_Position = p3;
        EmitVertex();
    }
    EndPrimitive();
}
