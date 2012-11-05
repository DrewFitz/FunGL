#ifndef __PRINTER_H__
#define __PRINTER_H__

#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include <GL/glfw.h>

#include "Shader.h"
#include "TextureUnit.h"

class Printer
{
	GLenum textTextureUnit, 
		   textureAtlasTextureUnit;
	Shader shader;
	GLuint VAO, 
		   vbo, 
		   textTexture, 
		   textureAtlas;
	float scale;

public:
	Printer();
	~Printer();

	void setScreenDimensions(int width, int height);
	void setPosition(int x, int y);
	void setScale(float scaleFactor);
	void setShader(const char* shaderName);
	void setTextureAtlas(const char* atlasName);

	void print(const char* text);
};

#endif /* __PRINTER_H__ */
