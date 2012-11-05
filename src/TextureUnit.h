#ifndef __TEXTUREUNIT_H__
#define __TEXTUREUNIT_H__

#include <OpenGL/gl3.h>

class TextureUnit
{
	static int nextFreeUnit;
public:
	static int getNextFreeTextureUnit();
};


#endif /* __TEXTUREUNIT_H__ */
