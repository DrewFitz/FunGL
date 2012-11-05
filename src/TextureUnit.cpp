#include "TextureUnit.h"

int TextureUnit::nextFreeUnit = 1;

int TextureUnit::getNextFreeTextureUnit()
{
	int temp = GL_MAX_TEXTURE_IMAGE_UNITS > nextFreeUnit ? nextFreeUnit : 0;
	++nextFreeUnit;
	return temp;
}
