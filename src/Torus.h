#ifndef __TORUS_H__
#define __TORUS_H__

#include "Drawable.h"
#include "Geometry.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Torus : public IndexedDrawable
{
	GLuint buffers[3];
	void generateGeometry();
	void bindGeometry();

public:
	Torus();
	~Torus();
	void update(int x, int y);
};

#endif /* __TORUS_H__ */
