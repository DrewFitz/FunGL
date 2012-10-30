#include "Model.h"

Model::Model()
{}

Model::Model(const char* name)
{
	loadModelByName(name);
}

Model::~Model()
{
	glDeleteVertexArrays(1, &VAO);
}

bool Model::loadModelByName(const char* name)
{
	char* filename = (char*)malloc(strlen(name) + strlen("resources/.model"));
	sprintf(filename, "resources/%s.model", name);

	bool result = loadModelByFilename(filename);

	free(filename);
	return result;
}

bool Model::loadModelByFilename(const char* filename)
{
	GLuint vbo;
	FILE *modelFile = fopen(filename, "r");

	unsigned int vertices = 0;
	if (modelFile == NULL)
	{
		printf("Failed to open model file.");
		return false;
	}
	int status = fscanf(modelFile, "%u\n", &vertices);

	if (status == EOF)
	{
		fclose(modelFile);
		return false;
	}

	vertexDataArrayLength = vertices * 3;
	GLfloat *vArray = new GLfloat[vertexDataArrayLength];
	for(unsigned int i=0; i < vertices; ++i)
	{
		int index = i * 3;
		float x, y, z;
		status = fscanf(modelFile, "%f %f %f\n", &x, &y, &z);
		if (status == EOF)
		{
			fclose(modelFile);
			break;
		}

		vArray[index]   = x;
		vArray[index+1] = y;
		vArray[index+2] = z;
	}
	fclose(modelFile);

	GLuint vertexLoc = 0;
	shader.loadShaderPairByName("debug");
	shader.compile();
	shader.makeActiveShaderProgram();
	vertexLoc = shader.getAttributeLocation("vVertex");
	printf("Vertex Loc: %u\n", vertexLoc);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &vbo);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexDataArrayLength, vArray, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexLoc);
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	delete[] vArray;
	return true;
}

bool Model::setShader(const char* shaderName)
{
	shader.loadShaderPairByName(shaderName);
	shader.compile();
	return true;
}
