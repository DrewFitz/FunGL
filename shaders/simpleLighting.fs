#version 150

in vec4 normal; // Model coordinates
in vec4 coords; // Model coordinates

uniform mat4 mMatrix 		= mat4(1.0);
uniform mat4 mvMatrix		= mat4(1.0);
uniform mat4 mvpMatrix		= mat4(1.0);
uniform mat4 pMatrix 		= mat4(1.0);
uniform mat4 normalMatrix	= mat4(1.0);

uniform float time 			= 0; // Time passed since launch (in seconds)
uniform int   specularPower = 1;

uniform vec4  lightPos 		= vec4(0.0, 10.0, 0.0, 1.0); // World coordinates
uniform vec4  cameraPos		= vec4(0.0, 0.0,  0.0, 1.0); // World coordinates

uniform vec4  diffuseColor	= vec4(0.4, 0.8, 0.8, 1.0);

out vec4 fragColor;		// Output to OpenGL


void main(void)
{
 	vec4
 		wCoords, 	// Point in world coordinates
 		wNormal,	// Transformed normal

 		lightDir,	// Direction from point to light
 		cameraDir,	// Direction from point to camera

 		reflection; // Light direction reflected across normal

	float 
		diffuse,	// Diffuse lighting brightness
		specular;	// Specular highlight brightness


	wCoords   = mMatrix * coords;
	wNormal   = normalize(normalMatrix * normal);

	lightDir  = normalize(lightPos  - wCoords);
	cameraDir = normalize(cameraPos - wCoords);

	// Reflection vector points the opposite direction than you think
	reflection = normalize(reflect(-lightDir, wNormal));


	specular = 1 - min(
		pow(
			1 - dot(cameraDir, reflection), 
			specularPower
		),
		1.0
	);

	diffuse  = max( 
		dot(lightDir, wNormal),
		0.0
	);
	

	fragColor = diffuse * diffuseColor + specular;
	//fragColor = vec4(specular);
}
