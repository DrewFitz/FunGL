#include "Torus.h"

Torus::Torus()
{
	drawMode = GL_TRIANGLE_STRIP;
	generateGeometry();
	setShader("simpleLighting");
}

Torus::Torus(const char* name)
{
	drawMode = GL_TRIANGLE_STRIP;
	generateGeometry();
	setShader(name);
}

Torus::~Torus()
{
	glDeleteBuffers(3, buffers);
	glDeleteVertexArrays(1, &VAO);
	free(normalDataArray);
	free(vertexDataArray);
	free(indexDataArray);
}

void Torus::generateGeometry()
{
	makeIndexedTorus(0.8f, 
					 0.2f, 
					 100, 
					 30, 
					 vertexDataArrayLength, 
					 vertexDataArray, 
					 normalDataArray, 
					 indexDataArrayLength, 
					 indexDataArray);
}

void Torus::bindGeometry()
{
	shader.makeActiveShaderProgram();
	GLuint vertexLoc = shader.getAttributeLocation("vVertex");
	GLuint normalLoc = shader.getAttributeLocation("vNormal");

	glGenVertexArrays(1, &VAO);
	glGenBuffers(3, buffers);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertexDataArrayLength, vertexDataArray, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexLoc);
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertexDataArrayLength, normalDataArray, GL_STATIC_DRAW);
	glEnableVertexAttribArray(normalLoc);
	glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indexDataArrayLength, indexDataArray, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Torus::setShader(const char* name)
{
	shader.loadShaderPairByName(name);
	shader.compile();

	bindGeometry();
}

void Torus::update(int x, int y)
{
	Matrix::Matrix 	xRotationMatrix, 
					yRotationMatrix, 
					projectionMatrix, 
					translationMatrix,
					scaleMatrix,
					normalMatrix,
					mvpMatrix;

	Matrix::Vector 	yRotationAxis(0.0f, 1.0f, 0.0f), 
					xRotationAxis(1.0f, 0.0f, 0.0f), 
					translationVec(0.0f, 0.0f, -10.0f);

	const float aspect = 16.0f / 9.0f;

	Matrix::makeIdentityMatrix(mvpMatrix);
	Matrix::makeRotationMatrix(xRotationMatrix, xRotationAxis, y/100.0f);
	Matrix::makeRotationMatrix(yRotationMatrix, yRotationAxis, x/100.0f);
	Matrix::makeTranslationMatrix(translationMatrix, translationVec);
	Matrix::makeUniformScalingMatrix(scaleMatrix, 5.0f);
	Matrix::makeProjectionMatrix(projectionMatrix, 90.0f, 0.1f, 100.0f, aspect);

	shader.makeActiveShaderProgram();
	glUniformMatrix4fv(shader.getUniformLocation("projection"), 1, GL_FALSE, projectionMatrix.m);

	// Model coordinates
	Matrix::matrixMultiply(mvpMatrix, mvpMatrix, scaleMatrix);
	// Avoid pseudo-gimbal lock by rotating around x axis first
	Matrix::matrixMultiply(mvpMatrix, mvpMatrix, xRotationMatrix);
	Matrix::matrixMultiply(mvpMatrix, mvpMatrix, yRotationMatrix); 
	Matrix::matrixMultiply(mvpMatrix, mvpMatrix, translationMatrix);
	glUniformMatrix4fv(shader.getUniformLocation("mMatrix"), 1, GL_FALSE, mvpMatrix.m);
	// World coordinates
	// View coordinates
	glUniformMatrix4fv(shader.getUniformLocation("mvMatrix"), 1, GL_FALSE, mvpMatrix.m);
	Matrix::matrixMultiply(mvpMatrix, mvpMatrix, projectionMatrix);
	// Clip coordinates
	glUniformMatrix4fv(shader.getUniformLocation("mvpMatrix"), 1, GL_FALSE, mvpMatrix.m);

	Matrix::makeIdentityMatrix(normalMatrix);
	// Model coords
	Matrix::matrixMultiply(normalMatrix, normalMatrix, xRotationMatrix);
	Matrix::matrixMultiply(normalMatrix, normalMatrix, yRotationMatrix);
	// World coords
	//Matrix::matrixMultiply(normalMatrix, normalMatrix, projectionMatrix);
	// Clip coords
	glUniformMatrix4fv(shader.getUniformLocation("normalMatrix"), 1, GL_FALSE, normalMatrix.m);

	glUseProgram(0);
}
