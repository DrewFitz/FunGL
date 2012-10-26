#ifndef __FRAMEBUFFEROBJECT_H__
#define __FRAMEBUFFEROBJECT_H__

#include <iostream>
#include <OpenGL/gl3.h>

class FramebufferObject
{
private:
	GLuint FBO, depthRBO, FBOTexture;
	int _width, _height;
	GLenum lastStatus = 0;
	void initialize();

public:
	FramebufferObject();
	FramebufferObject(int width, int height);
	~FramebufferObject();


	void makeActiveFramebuffer();
	void bindToTextureUnit(int unit);
	bool isValid();
	GLenum status();

};

#endif /* __FRAMEBUFFEROBJECT_H__ */
