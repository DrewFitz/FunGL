#ifndef __FRAMEBUFFEROBJECT_H__
#define __FRAMEBUFFEROBJECT_H__

#include <iostream>
#include <OpenGL/gl3.h>

class FramebufferObject
{
private:
	GLuint FBO 		  = 0, 
		   depthRBO   = 0, 
		   FBOTexture = 0;
	GLenum lastStatus = 0;
	int _width, _height;
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
