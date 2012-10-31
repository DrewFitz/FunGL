#include "FramebufferObject.h"

void FramebufferObject::initialize()
{
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Setup texture target for FBO rendering
	glGenTextures(1, &FBOTexture);
	glBindTexture(GL_TEXTURE_2D, FBOTexture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBOTexture, 0);

	// Setup renderbuffer to hold depth
	glGenRenderbuffers(1, &depthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, _width, _height);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRBO);

	if (!isValid())
	{
		switch(lastStatus)
		{
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
				printf("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT\n"); 
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:    
				printf("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT\n"); 
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:    
				printf("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER\n"); 
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:    
				printf("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER\n"); 
				break;
			case GL_FRAMEBUFFER_UNDEFINED:
				printf("GL_FRAMEBUFFER_UNDEFINED\n"); 
				break;
			case GL_FRAMEBUFFER_UNSUPPORTED:
				printf("GL_FRAMEBUFFER_UNSUPPORTED\n"); 
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
				printf("GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE\n"); 
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
				printf("GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS\n"); 
				break;
			case 0:
				printf("Some error occurred apparently.\n");
				break;

			default:
				printf("WTF?!\n");
				break;
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

FramebufferObject::FramebufferObject() : _width(512), _height(512)
{
	initialize();
}

FramebufferObject::FramebufferObject(int width, int height) : _width(width), _height(height)
{
	initialize();
}

FramebufferObject::~FramebufferObject()
{
	glDeleteRenderbuffers(1, &depthRBO);
	glDeleteTextures(1, &FBOTexture);
	glDeleteFramebuffers(1, &FBO);
}

void FramebufferObject::makeActiveFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glViewport(0, 0, _width, _height);
}

void FramebufferObject::bindToTextureUnit(int unit)
{
	if (unit > GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS - 1 || unit == 0)
	{
		printf("Invalid texture unit %d requested for binding FBO %u\n", unit, FBO);
		return;
	}
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, FBOTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool FramebufferObject::isValid()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	lastStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (lastStatus == GL_FRAMEBUFFER_COMPLETE)
	{
		return true;
	} else {
		return false;
	}
}
GLenum FramebufferObject::status()
{
	return lastStatus;
}
