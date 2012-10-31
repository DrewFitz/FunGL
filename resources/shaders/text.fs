#version 150

in vec2 uv;

uniform sampler1D textTexture;
uniform sampler2D textAtlas;

uniform ivec2 atlasDimensions;
uniform ivec2 atlasCellDimensions;

uniform vec2 drawLocation; // pixel location
uniform vec2 screenDimensions;
uniform float scale;

out vec4 fragColor;

void main(void)
{
	fragColor.rgb = texture(textAtlas, uv).rgb;

	fragColor.a = 1.0;
}
