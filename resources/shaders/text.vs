#version 150

in vec4 vVertex;

uniform sampler1D textTexture;
uniform sampler2D textAtlas;

uniform ivec2 atlasDimensions;
uniform ivec2 atlasCellDimensions;

uniform vec2 drawLocation; // pixel location
uniform vec2 screenDimensions;
uniform float scale;

out vec2 uv;

void main(void)
{
	int index = gl_InstanceID;

	int letter = int(floor(texture(textTexture, index).r));
	// Letter to cell
	ivec2 atlasCell = ivec2(mod(letter, atlasDimensions.x), 
						    int(floor(letter / atlasDimensions.x)));
	// cell to texel space
	uv.x = (vVertex.x + atlasCell.x * atlasCellDimensions.x) / atlasDimensions.x;
	uv.y = (atlasDimensions.y - (vVertex.y + atlasCell.y * atlasCellDimensions.y)) / atlasDimensions.y;

	// correct (pretty sure)
	gl_Position.x = vVertex.x * index * scale + drawLocation.x;
	gl_Position.y = vVertex.y * scale + drawLocation.y;

	// scale from screen pixel space to 0..1
	gl_Position.x = gl_Position.x / screenDimensions.x;
	gl_Position.y = gl_Position.y / screenDimensions.y;
	// transform to clip space -1..1
	gl_Position.x = gl_Position.x * 2.0 - 1.0;
	gl_Position.y = gl_Position.y * 2.0 - 1.0;

	gl_Position.z = 0.0; // possibly wrong
	gl_Position.w = 1.0; // ?
}
