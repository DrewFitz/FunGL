#version 150

in vec4 normal; // World coordinates
in vec4 coords; // World coordinates

uniform mat4 mMatrix;
uniform mat4 mvMatrix;
uniform mat4 mvpMatrix;
uniform mat4 pMatrix;
uniform mat4 normalMatrix;

uniform float time 			= 0; // Time passed since launch (in seconds)
uniform int   specularPower = 1;

uniform vec3  lightPos 		= vec3(0.0, 0.0, 0.0); // World coordinates
uniform vec3  cameraPos		= vec3(0.0, 0.0, 0.0); // World coordinates

uniform vec4  diffuseColor	= vec4(0.4, 0.8, 0.8, 1.0);

out vec4 fragColor;		// Output to OpenGL


void main(void)
{
 	vec3
 		lightDir,	// Direction from point to light
 		cameraDir,	// Direction from point to camera

 		reflection; // Light direction reflected across normal

	float 
		diffuse,	// Diffuse lighting brightness
		specular;	// Specular highlight brightness

	lightDir  = normalize(lightPos  - coords.xyz);
	cameraDir = normalize(cameraPos - coords.xyz);

	// Reflection vector points the opposite direction than you think
	reflection = normalize(reflect(-lightDir, normal.xyz));

	float spec = specularPower < 0.0 ? 1.0 / -specularPower : specularPower;
	spec = spec == 0.0 ? 1.0 : spec; // This probably isn't necessary in practice, it just makes the demo functionality look nicer
	specular = max(
		pow(
			dot(cameraDir, reflection), 
			spec
		),
		0.0
	);

	diffuse  = max( 
		dot(lightDir, normal.xyz),
		0.0
	);
	

	fragColor = vec4(vec3(diffuse), 1.0) * diffuseColor;
	fragColor.rgb += specular;
}
