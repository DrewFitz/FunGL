#version 150

in vec4 vVertex;
in vec4 vNormal;


uniform mat4 mMatrix 		= mat4(1.0);
uniform mat4 mvMatrix		= mat4(1.0);
uniform mat4 mvpMatrix		= mat4(1.0);
uniform mat4 projection   	= mat4(1.0);
uniform mat4 normalMatrix	= mat4(1.0);

uniform float time;
uniform int   specularPower;

uniform vec4  lightPos;
uniform vec4  camera;

uniform vec4  diffuseColor;

out vec4 normal;
out vec4 coords;

void main(void)
{
	coords 		= mMatrix * vVertex;
	normal 		= normalMatrix * vNormal;
	gl_Position = mvpMatrix * vVertex;
}
