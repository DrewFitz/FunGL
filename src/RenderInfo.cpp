#include "RenderInfo.h"

int RenderInfo::width = 0;
int RenderInfo::height = 0;

RenderInfo::RenderInfo() {}
RenderInfo::~RenderInfo() {}

int RenderInfo::getWidth() {
	return width;
}

int RenderInfo::getHeight() {
	return height;
}
