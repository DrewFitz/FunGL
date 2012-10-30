#version 150

in vec4 vVertex;
uniform mat4 mvpMatrix = mat4(1.0);

void main(void)
{
	gl_Position = mvpMatrix * vVertex;
}
