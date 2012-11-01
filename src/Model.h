#ifndef __MODEL_H__
#define __MODEL_H__

#include "Drawable.h"

class Model : public Drawable {

public:
	Model();
	Model(const char* file);
	~Model();

	bool loadModelByName(const char* name);
	bool loadModelByFilename(const char* filename);
	bool setShader(const char* shaderName);
};

#endif /* __MODEL_H__ */
