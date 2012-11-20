#include "Torus.h"

Torus::Torus()
{
	drawMode = GL_TRIANGLE_STRIP;
	generateGeometry();
	shader.loadShaderPairByName("simpleLighting");
	shader.compile();
	bindGeometry();
}

Torus::~Torus()
{
	glDeleteBuffers(3, buffers);
	glDeleteVertexArrays(1, &VAO);
	delete[] normalDataArray;
	delete[] vertexDataArray;
	delete[] indexDataArray;
}

void Torus::generateGeometry()
{
	Geometry::makeIndexedTorus(	0.8f, 
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
	GLuint vertexLoc = shader.getAttributeLocation("vVertex"),
		   normalLoc = shader.getAttributeLocation("vNormal");

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

void Torus::update(int x, int y)
{
	static glm::mat4 	xRotationMatrix, 
						yRotationMatrix, 
						pMatrix, 
						tMatrix,
						scaleMatrix,
						nMatrix,
						mvpMatrix;

	static glm::vec3 	yAxis			(  0.0f,  1.0f,  0.0f ), 
						xAxis			(  1.0f,  0.0f,  0.0f ), 
						translationVec	(  0.0f,  0.0f, -5.0f ),
						scaleVec		(  1.0f,  1.0f,  1.0f );

	nMatrix = mvpMatrix = glm::mat4(1.0f);
	xRotationMatrix = glm::rotate(glm::mat4(1.0f), y/10.0f, xAxis);
	yRotationMatrix = glm::rotate(glm::mat4(1.0f), x/10.0f, yAxis);
	tMatrix 		= glm::translate(glm::mat4(1.0f), translationVec);
	scaleMatrix		= glm::scale(scaleVec);
	pMatrix 		= glm::perspective(45.0f, 16.0f/9.0f, 0.1f, 100.0f);

	shader.makeActiveShaderProgram();
	glUniformMatrix4fv(shader.getUniformLocation("pMatrix"), 1, GL_FALSE, glm::value_ptr(pMatrix));

	// Model coordinates
	// Avoid pseudo-gimbal lock by rotating around x axis first
	mvpMatrix = glm::mat4(1.0f)
				* tMatrix
				* yRotationMatrix 
				* xRotationMatrix 
				* scaleMatrix
				* mvpMatrix 
				;
	glUniformMatrix4fv(shader.getUniformLocation("mMatrix"), 1, GL_FALSE, glm::value_ptr(mvpMatrix));
	// World coordinates
	// View coordinates
	glUniformMatrix4fv(shader.getUniformLocation("mvMatrix"), 1, GL_FALSE, glm::value_ptr(mvpMatrix));
	mvpMatrix = pMatrix * mvpMatrix;
	// Clip coordinates
	glUniformMatrix4fv(shader.getUniformLocation("mvpMatrix"), 1, GL_FALSE, glm::value_ptr(mvpMatrix));

	// Model coords
	nMatrix = nMatrix * yRotationMatrix * xRotationMatrix;
	// World coords
	glUniformMatrix4fv(shader.getUniformLocation("nMatrix"), 1, GL_FALSE, glm::value_ptr(nMatrix));

	glUseProgram(0);
}
