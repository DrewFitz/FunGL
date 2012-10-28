#ifndef __SHADER_H__
#define __SHADER_H__

#include <stdlib.h>
#include <iostream>
#include <OpenGL/gl3.h>

class Shader
{
public:
	GLuint program;
	GLuint vertexShader;
	GLuint fragmentShader;


	Shader() : program{0}, vertexShader{0}, fragmentShader{0} {}
	Shader(const char * const shaderName);
	Shader(const char * const vertexFilename, const char * const fragmentFilename);
	~Shader();
	Shader(Shader&& other);

	bool loadVertexShader(const char * filename);
	bool loadFragmentShader(const char * filename);
	bool loadShaderPairByName(const char* name);
	
	bool compile();
	bool makeActiveShaderProgram();
	bool validate();

	inline GLuint getUniformLocation(const char * name) const;
	inline GLuint getAttributeLocation(const char * name) const;

private:
	GLuint loadShader(const char * const filename, GLenum shaderType);
	// Delete copy constructor
	Shader(const Shader&)=delete;



};
inline GLuint Shader::getUniformLocation(const char * name) const
{
  return glGetUniformLocation(program, name);
}
inline GLuint Shader::getAttributeLocation(const char * name) const
{
  return glGetAttribLocation(program, name);
}

#endif /* __SHADER_H__ */
