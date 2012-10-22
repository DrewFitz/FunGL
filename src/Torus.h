#ifndef __TORUS_H__
#define __TORUS_H__

#include "Drawable.h"
#include "Geometry.h"
#include "Matrix.h"

class Torus : public IndexedDrawable
{
	GLuint buffers[3];
	void generateGeometry();
	void bindGeometry();

public:
	Torus();
	Torus(const char* name);
	~Torus();
	void setShader(const char* name);
	void update(int x, int y);
	virtual void draw();
};

#endif /* __TORUS_H__ */
