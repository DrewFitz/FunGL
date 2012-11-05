#version 150

in vec4 vVertex;

uniform sampler1D textTexture;
uniform sampler2D textAtlas;

uniform ivec2 atlasDimensions;
uniform ivec2 atlasCellDimensions;

uniform vec2 drawLocation; // pixel location
uniform ivec2 screenDimensions; // pixels
uniform float scale; // scale of letter quads
uniform int stringLength;

out vec2 uv;

void main(void)
{
	int index = gl_InstanceID;
	// Sample from the MIDDLE of the pixel!
	float letterTexCoord = (float(index) + 0.5) / 255;

	int letter = int(255 * texture(textTexture, letterTexCoord).r);

	// Letter to cell
	ivec2 atlasCells = atlasDimensions / atlasCellDimensions;
	ivec2 thisCell = ivec2(mod(letter, atlasCells.x), letter / atlasCells.x);

	// cell to texel space
	// bottom space is cut off due to extra space around grid in texture
	float yOffset = 1.0 - (float(atlasCells.y) * atlasCellDimensions.y / atlasDimensions.y);
	uv.x = (vVertex.x + thisCell.x) * atlasCellDimensions.x / atlasDimensions.x;
	uv.y = yOffset + (vVertex.y - 1 + atlasCells.y - thisCell.y) * atlasCellDimensions.y / atlasDimensions.y;


	vec2 realScale = scale * atlasCellDimensions;
	gl_Position.x = (vVertex.x * realScale.x) + (index * realScale.x) + drawLocation.x;
	gl_Position.y = (vVertex.y * realScale.y) + drawLocation.y;

	// scale from screen pixel space to 0..1
	gl_Position.x = gl_Position.x / screenDimensions.x;
	gl_Position.y = gl_Position.y / screenDimensions.y;
	// transform to clip space -1..1
	gl_Position.x = gl_Position.x * 2.0 - 1.0;
	gl_Position.y = gl_Position.y * 2.0 - 1.0;

	gl_Position.z = -1.0; 
	gl_Position.w = 1.0;
}
