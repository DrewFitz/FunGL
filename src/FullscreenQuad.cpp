#include "FullscreenQuad.h"

FullscreenQuad::FullscreenQuad()
{
	generateGeometry();
	setShader("postProcessing");
}

FullscreenQuad::FullscreenQuad(const char* name)
{
	generateGeometry();
	setShader(name);
}

FullscreenQuad::~FullscreenQuad()
{
	glDeleteBuffers(2, buffers);
	glDeleteVertexArrays(1, &VAO);
	delete[] vertexDataArray;
	delete[] indexDataArray;
}

void FullscreenQuad::generateGeometry()
{
	vertexDataArrayLength = 12;
	indexDataArrayLength  = 4;

	if (vertexDataArray != nullptr)
	{
		delete[] vertexDataArray;
	}
	if (indexDataArray != nullptr)
	{
		delete[] indexDataArray;
	}

	vertexDataArray = new GLfloat[vertexDataArrayLength];
	indexDataArray = new GLuint[indexDataArrayLength];

	// Clip coordinates
	const static GLfloat tempVertices[] = {
		-1.0,  1.0,  0.0,
		-1.0, -1.0,  0.0,
		 1.0,  1.0,  0.0,
		 1.0, -1.0,  0.0
	};
	const static GLuint tempIndices[] = {
		0, 1, 2, 3
	};

	memcpy(vertexDataArray, tempVertices, sizeof(GLfloat)*vertexDataArrayLength);
	memcpy(indexDataArray, tempIndices, sizeof(GLint)*indexDataArrayLength);
}

void FullscreenQuad::bindGeometry()
{
	shader.makeActiveShaderProgram();
	GLuint vertexLoc = shader.getAttributeLocation("vVertex");

	glGenVertexArrays(1, &VAO);
	glGenBuffers(2, buffers);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertexDataArrayLength, vertexDataArray, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexLoc);
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indexDataArrayLength, indexDataArray, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void FullscreenQuad::setShader(const char* name)
{
	shader.loadShaderPairByName(name);
	shader.compile();

	bindGeometry();
}

void FullscreenQuad::draw()
{
	shader.makeActiveShaderProgram();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLE_STRIP, indexDataArrayLength, GL_UNSIGNED_INT, 0); 
	glBindVertexArray(0);
	glUseProgram(0);
}
