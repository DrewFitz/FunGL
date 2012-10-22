#ifndef __DRAWABLE_H__
#define __DRAWABLE_H__

#include <OpenGL/gl3.h>
#include "Shader.h"

class Drawable
{
protected:
	GLuint VAO;
	GLfloat* vertexDataArray = nullptr;
	GLfloat* normalDataArray = nullptr;
	unsigned int vertexDataArrayLength;

public:
	Shader shader;
	virtual void draw()=0;
	virtual ~Drawable() {}
};

class IndexedDrawable : public Drawable
{
protected:
	GLuint* indexDataArray = nullptr;
	unsigned int indexDataArrayLength;
	
public:
	virtual ~IndexedDrawable() {}
};


#endif /* __DRAWABLE_H__ */
