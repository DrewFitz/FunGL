#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <OpenGL/gl3.h>

#include <stdlib.h>
#include <cmath>

#ifndef PI
#define PI (3.141592f)
#endif // PI

inline void makeTorus(float radius, 
					  float width, 
					  unsigned segments, 
					  unsigned aroundSegments, 
					  unsigned int& length, 
					  GLfloat*& out);
inline void makeIndexedTorus(float radius, 
							 float width, 
							 unsigned int segments, 
							 unsigned int aroundSegments, 
							 unsigned int& verticesLength, 
							 GLfloat*& vertices, 
							 GLfloat*& normals, 
							 unsigned int& indicesLength, 
							 GLuint*& indices);
inline void makeFlatLoop(float radius, float width, unsigned segments, unsigned int& length, GLfloat*& out);

inline void makeTorus(
	float radius, 
	float width, 
	unsigned segments, 
	unsigned aroundSegments, 
	unsigned int& length, 
	GLfloat*& out)
{
	const unsigned int	rotationalSteps 	= segments * aroundSegments;
	const float 		rotationalStepAngle = float(2*PI/rotationalSteps);
	const float 		circleStepAngle		= float(2*PI/aroundSegments);

	length = 3*2*rotationalSteps;
	out = (GLfloat*)malloc(sizeof(GLfloat)*length);

	for (unsigned int i = 0; i < rotationalSteps; ++i)
	{
		unsigned int index = i*6;

		float theta = rotationalStepAngle * i;
		float gamma = circleStepAngle * (i % aroundSegments);

		out[index]   = sinf(theta) * (radius + sinf(gamma) * width); // X1
		out[index+2] = cosf(theta) * (radius + sinf(gamma) * width); // Z1

		theta = rotationalStepAngle * (i + aroundSegments);

		out[index+3] = sinf(theta) * (radius + sinf(gamma) * width); // X2
		out[index+5] = cosf(theta) * (radius + sinf(gamma) * width); // Z2

		out[index+1] = out[index+4] = cosf(gamma) * width; // Y1 Y2
	}
}

/* Make Indexed Torus
 * returns: void
 * parameters:
 * float radius - the distance from the center of the torus to the center of the torus tube
 * float width  - the width of the torus tube
 * unsigned int segments - how many points go around the torus
 * unsigned int aroundSegments - how many points go around the torus tube
 * unsigned int& verticesLength - length of vertices array
 * GLfloat*& vertices - array of vertices that make up the torus
 * GLfloat*& normals  - array of vertex normals (same length as vertices array)
 * unsigned int& indicesLength - length of indices array
 * GLuint*& indices   - array of triangle indices
 *
 * variables:
 * const unsigned int rotationalSteps - number of steps to take around torus
 * const float 		  rotationalStepAngle - the angle delta for steps around torus
 * const float 		  circleStepAngle - angle delta for steps around torus bands
 *
 */
inline void makeIndexedTorus(
	float radius, 
	float width, 
	unsigned int segments, 
	unsigned int aroundSegments, 
	unsigned int& verticesLength, 
	GLfloat*& vertices, 
	GLfloat*& normals,
	unsigned int& indicesLength, 
	GLuint*& indices)
{
	const unsigned int	rotationalSteps 	= segments * aroundSegments;
	const float 		rotationalStepAngle = float(2*PI/rotationalSteps);
	const float 		circleStepAngle		= float(2*PI/aroundSegments);

	verticesLength = 3*rotationalSteps;
	indicesLength = 2*(rotationalSteps+1);
	vertices = (GLfloat*)malloc(sizeof(GLfloat)*verticesLength);
	normals  = (GLfloat*)malloc(sizeof(GLfloat)*verticesLength);
	indices  = (GLuint*)malloc(sizeof(GLuint)*indicesLength);

	for (unsigned int i = 0; i < rotationalSteps; ++i)
	{
		unsigned int index = i*3;

		float theta = rotationalStepAngle * i;
		float gamma = circleStepAngle * (i % aroundSegments);

		vertices[index]   = sinf(theta) * (radius + sinf(gamma) * width); // X
		vertices[index+1] = cosf(gamma) * width; 						  // Y
		vertices[index+2] = cosf(theta) * (radius + sinf(gamma) * width); // Z

		normals[index]	  = sinf(theta) * sinf(gamma);	// X
		normals[index+1]  = cosf(gamma);				// Y
		normals[index+2]  = cosf(theta) * sinf(gamma); 	// Z
	}

	for (unsigned int i = 0; i < rotationalSteps+1; ++i)
	{
		unsigned int index = i*2;
		indices[index] = (i+aroundSegments) % (verticesLength/3);
		indices[index+1] = i % (verticesLength/3);
	}
}

inline void makeFlatLoop(float radius, float width, unsigned segments, unsigned int& length, GLfloat*& out)
{
	const float rotational_step = (2.0f*PI/segments);

	length = 3*2*(segments+1);
	out = (GLfloat*)malloc(sizeof(GLfloat) * length);

	for(unsigned int i=0; i<=segments; ++i)
	{
		int vertex = i*6;
		out[vertex+3] = out[vertex] = sin(i*rotational_step) * radius;
		out[vertex+1] = width/2;
		out[vertex+5] = out[vertex+2] = cos(i*rotational_step) * radius;
		out[vertex+4] = -width/2;
	}
}

#endif /* __GEOMETRY_H__ */
