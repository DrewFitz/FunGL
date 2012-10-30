#version 150
in vec4 color;
in vec2 texCoord;
in float diffuse;
    
in vec4 eye_pos;
in vec3 normal;
in vec3 light_direction0;
in vec3 light_direction1;
in vec3 light_direction2;

out vec4 fragColor;


// lighting
vec4 ambientSceneLightColor		= vec4(.6, .6, .6, 1.0);
vec4 ambientMaterial			= vec4(.2, .2, .2, 1.0);
vec4 lightSourceAmbientColor0	= vec4(.3, .2, .1, 1.0);
vec4 lightSourceAmbientColor1	= vec4(.2, .3, .1, 1.0);
vec4 lightSourceAmbientColor2	= vec4(.1, .2, .3, 1.0);

vec4 lightSourceDiffuseColor0	= vec4(.7, .3, .2, 1.0);
vec4 lightSourceDiffuseColor1	= vec4(.3, .7, .2, 1.0);
vec4 lightSourceDiffuseColor2	= vec4(.2, .3, .7, 1.0);
vec4 diffuseMaterial			= vec4(.4, .4, .4, 1.0);

vec4 lightSourceSpecularColor0	= vec4(.7, .5, .5, 1.0); 
vec4 lightSourceSpecularColor1	= vec4(.5, .7, .5, 1.0);
vec4 lightSourceSpecularColor2	= vec4(.5, .5, .7, 1.0);
vec4 specularMaterial			= vec4(.8, .8, .8, 1.0);

float materialShininess = 5.0;

void main()
{
	vec4 ambientColor = ambientSceneLightColor * ambientMaterial +
						lightSourceAmbientColor0 * ambientMaterial +
						lightSourceAmbientColor1 * ambientMaterial +
						lightSourceAmbientColor2 * ambientMaterial;
	
	vec4 diffuseColor = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 specularColor = vec4(0.0, 0.0, 0.0, 0.0);
	
	float lambertian0 = dot(normal.xyz, light_direction0);
	float lambertian1 = dot(normal.xyz, light_direction1);
	float lambertian2 = dot(normal.xyz, light_direction2);
	
	vec3 eye_dir = normalize(eye_pos.xyz);
	
	// light 0
	if (lambertian0 > 0.0)
	{
		diffuseColor += lightSourceDiffuseColor0 * diffuseMaterial * lambertian0;
		
		vec3 reflection_dir0 = normalize(reflect(light_direction0, normal.xyz));
		float coherence = dot(reflection_dir0, eye_dir);
		float specular_term = pow ( max(coherence, 0.0),  materialShininess);
		
		specularColor += lightSourceSpecularColor0 * specularMaterial * specular_term;
	}
	
	// light 1
	if (lambertian1 > 0.0)
	{
		diffuseColor += lightSourceDiffuseColor1 * diffuseMaterial * lambertian1;
		
		vec3 reflection_dir1 = normalize(reflect(light_direction1, normal.xyz));
		float coherence = dot(reflection_dir1, eye_dir);
		float specular_term = pow ( max(coherence, 0.0),  materialShininess);
		
		specularColor += lightSourceSpecularColor1 * specularMaterial * specular_term;
	}
	
	// light 2
	if (lambertian2 > 0.0)
	{
		diffuseColor += lightSourceDiffuseColor2 * diffuseMaterial * lambertian2;
		
		vec3 reflection_dir2 = normalize(reflect(light_direction2, normal.xyz));
		float coherence = dot(reflection_dir2, eye_dir);
		float specular_term = pow ( max(coherence, 0.0),  materialShininess);
		
		specularColor += lightSourceSpecularColor2 * specularMaterial * specular_term;
	}
	
	vec4 final_color = ambientColor + diffuseColor + specularColor;
	
	fragColor = clamp(final_color, 0.0, 1.0);
}