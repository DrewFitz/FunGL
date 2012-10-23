#version 150

in vec4 normal; // World coordinates
in vec4 coords; // World coordinates

uniform mat4 mMatrix 		= mat4(1.0);
uniform mat4 mvMatrix		= mat4(1.0);
uniform mat4 mvpMatrix		= mat4(1.0);
uniform mat4 pMatrix 		= mat4(1.0);
uniform mat4 normalMatrix	= mat4(1.0);

uniform float time 			= 0; // Time passed since launch (in seconds)
uniform int   specularPower = 1;

uniform vec4  lightPos 		= vec4(0.0, 0.0, 0.0, 1.0); // World coordinates
uniform vec4  cameraPos		= vec4(0.0, 0.0, 0.0, 1.0); // World coordinates

uniform vec4  diffuseColor	= vec4(0.4, 0.8, 0.8, 1.0);

out vec4 fragColor;		// Output to OpenGL


void main(void)
{
 	vec4
 		lightDir,	// Direction from point to light
 		cameraDir,	// Direction from point to camera

 		reflection; // Light direction reflected across normal

	float 
		diffuse,	// Diffuse lighting brightness
		specular;	// Specular highlight brightness

	lightDir  = normalize(lightPos  - coords);
	cameraDir = normalize(cameraPos - coords);

	// Reflection vector points the opposite direction than you think
	reflection = normalize(reflect(-lightDir, normal));


	float spec = specularPower < 0.0 ? 1.0 / -specularPower : specularPower;
	spec = spec == 0.0 ? 1.0 : spec;
	specular = max(
		pow(
			dot(cameraDir, reflection), 
			spec
		),
		0.0
	);

	diffuse  = max( 
		dot(lightDir, normal),
		0.0
	);
	

	//fragColor = diffuse * diffuseColor + specular;
	fragColor = vec4(specular);
}
