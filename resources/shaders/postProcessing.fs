/*
I guess I should note that this version of postProcessing.fs is an adapted
version of iq's "Postpro" shader from http://www.iquilezles.org/apps/shadertoy/
*/
#version 150

in vec4 vertex;
uniform sampler2D tex0;
uniform float time;
out vec4 fragColor;

void main(void)
{
	// Screen coordinate 0..1
	vec2 pixel = vertex.xy * 0.5 + 0.5;
	vec3 color;

	// Slow pulsing
	pixel = 0.5 + (pixel - 0.5) * (0.9 + 0.1 * sin(0.2 * time));

	// Load and shift colors
	color.r = texture(tex0, vec2(pixel.x + 0.001, pixel.y)).r;
	color.g = texture(tex0, vec2(pixel.x + 0.000, pixel.y)).g;
	color.b = texture(tex0, vec2(pixel.x - 0.001, pixel.y)).b;


	// Increase contrast
	color = clamp(color * 0.1 + 0.9 * color * color, 0.0, 1.0);
	// Vignette
	color *= 0.5 + 8.0 * pixel.x * (1.0 - pixel.x) * pixel.y * (1.0 - pixel.y);
	// Green tint
	color *= vec3(0.8, 1.0, 0.7);
	// Scanlines
	color *= 0.9 + 0.1 * sin(10.0 * time + pixel.y * 500.0);
	// Screen flicker
	color *= 0.97 + 0.03 * sin(110.0 * time);

	fragColor = vec4(color, 1.0);

}
