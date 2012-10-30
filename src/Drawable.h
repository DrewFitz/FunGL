#ifndef __DRAWABLE_H__
#define __DRAWABLE_H__

#include <OpenGL/gl3.h>
#include "Shader.h"

class Drawable
{
protected:
	GLuint VAO;
	GLenum drawMode = GL_TRIANGLES;
	GLfloat* vertexDataArray = nullptr;
	GLfloat* normalDataArray = nullptr;
	unsigned int vertexDataArrayLength;

public:
	Shader shader;
	Drawable() : VAO(0), vertexDataArrayLength(0) {}
	virtual ~Drawable() {}
	virtual void draw() {
		shader.makeActiveShaderProgram();
		glBindVertexArray(VAO);
		glDrawArrays(drawMode, 0, vertexDataArrayLength);
		glBindVertexArray(0);
		glUseProgram(0);
	}
};

class IndexedDrawable : public Drawable
{
protected:
	GLuint* indexDataArray = nullptr;
	unsigned int indexDataArrayLength;
	
public:
	IndexedDrawable() : indexDataArrayLength(0) {}
	virtual ~IndexedDrawable() {}
	virtual void draw() {
		shader.makeActiveShaderProgram();
		glBindVertexArray(VAO);
		glDrawElements(drawMode, indexDataArrayLength, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}
};


#endif /* __DRAWABLE_H__ */
