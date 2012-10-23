#version 150

in vec4 vVertex;
in vec4 vNormal;


uniform mat4 mMatrix 		= mat4(1.0);
uniform mat4 mvMatrix		= mat4(1.0);
uniform mat4 mvpMatrix		= mat4(1.0);
uniform mat4 projection   	= mat4(1.0);
uniform mat4 normalMatrix	= mat4(1.0);

uniform float time 			= 0; // Time passed since launch (in seconds)
uniform int   specularPower = 1;

uniform vec4  lightPos 		= vec4(0.0, 0.0, 0.0, 1.0); // World coordinates
uniform vec4  camera 		= vec4(0.0, 0.0, 0.0, 1.0); // World coordinates

uniform vec4  diffuseColor	= vec4(0.4, 0.8, 0.8, 1.0);

out vec4 normal;
out vec4 coords;

void main(void)
{
	coords 		= mMatrix * vVertex;
	normal 		= normalMatrix * vNormal;
	gl_Position = mvpMatrix * vVertex;
}
