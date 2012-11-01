#ifndef __PRINTER_H__
#define __PRINTER_H__

#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include <GL/glfw.h>

#include "Shader.h"

class Printer
{
	Shader shader;
	GLuint VAO, vbo, textTexture, textureAtlas;
	int _x, _y;
public:
	Printer();
	~Printer();

	void setPosition(int x, int y);
	void setShader(const char* shaderName);
	void setTextureAtlas(const char* atlasName);

	void print(const char* text);

};

#endif /* __PRINTER_H__ */

