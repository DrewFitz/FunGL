#ifndef __RENDERINFO_H__
#define __RENDERINFO_H__

class RenderInfo
{
public:
	RenderInfo();
	~RenderInfo();

	static int width, height;
	static int getWidth();
	static int getHeight();

};

#endif /* __RENDERINFO_H__ */
