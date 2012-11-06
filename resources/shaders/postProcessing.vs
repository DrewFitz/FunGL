#version 150

in vec4 vVertex;
out vec4 vertex;

void main(void)
{
	vertex = vVertex;
	gl_Position = vVertex;
}