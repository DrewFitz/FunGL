#version 150

out vec4 fragColor;

void main(void)
{
	fragColor.ba = vec2(1.0, 1.0);
	fragColor.rg = gl_FragCoord.xy / vec2(1280, 720);
}