#ifndef __FULLSCREENQUAD_H__
#define __FULLSCREENQUAD_H__

#include "Drawable.h"

class FullscreenQuad : public IndexedDrawable
{
	GLuint buffers[2]; 
	void generateGeometry();
	void bindGeometry();
public:
	FullscreenQuad();
	FullscreenQuad(const char* name);
	~FullscreenQuad();

	void setShader(const char* name);
};


#endif /* __FULLSCREENQUAD_H__ */
