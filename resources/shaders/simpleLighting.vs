#version 150


in vec4 vVertex;
in vec4 vNormal;

uniform mat4 mMatrix 		= mat4(1.0);
uniform mat4 mvpMatrix		= mat4(1.0);
uniform mat4 nMatrix		= mat4(1.0);

uniform float time;
uniform int   specularPower;

uniform vec3  lightPos;
uniform vec3  cameraPos;

uniform vec4  diffuseColor;

out vec4 normal;
out vec4 coords;


void main(void)
{
	coords 		= mMatrix * vVertex;
	gl_Position = mvpMatrix * vVertex;
	normal 		= nMatrix * vNormal;
}
