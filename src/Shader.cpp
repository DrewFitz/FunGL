#include "Shader.h"

/*
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
  void validate();

  void getLastError(char *& err);
  inline GLuint getUniformLocation(const char * name) const;
  inline GLuint getAttributeLocation(const char * name) const;

private:
  GLuint loadShader(const char * const filename, GLenum shaderType);
  Shader(const Shader&)=delete;
};
*/

Shader::Shader(const char * const shaderName)
{
  loadShaderPairByName(shaderName);
}
Shader::Shader(const char * const vertexFilename, const char * const fragmentFilename)
{
  loadVertexShader(vertexFilename);
  loadFragmentShader(fragmentFilename);
}
Shader::~Shader()
{
  glDetachShader(program, vertexShader);
  glDetachShader(program, fragmentShader);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteProgram(program);
}
Shader::Shader(Shader&& other)
{
  program = other.program;
  other.program = 0;
  vertexShader = other.vertexShader;
  other.vertexShader = 0;
  fragmentShader = other.fragmentShader;
  other.fragmentShader = 0;
}

bool Shader::loadVertexShader(const char * filename)
{
  if (vertexShader)
  {
    glDeleteShader(vertexShader);
  }
  vertexShader = loadShader(filename, GL_VERTEX_SHADER);
  if (vertexShader != 0)
  {
    return true;
  } else {
    return false;
  }
}
bool Shader::loadFragmentShader(const char * filename)
{
  if (fragmentShader)
  {
    glDeleteShader(fragmentShader);
  }
  fragmentShader = loadShader(filename, GL_FRAGMENT_SHADER);
  if (fragmentShader != 0)
  {
    return true;
  } else {
    return false;
  }
}
bool Shader::loadShaderPairByName(const char* name)
{
  const unsigned int length = strlen(name);
  char* vsName = (char*)malloc(length+3);
  char* fsName = (char*)malloc(length+3);
  memcpy(vsName, name, strlen(name));
  memcpy(fsName, name, strlen(name));
  sprintf(vsName, "shaders/%s%s", name, ".vs");
  sprintf(fsName, "shaders/%s%s", name, ".fs");
  if(loadVertexShader(vsName) && loadFragmentShader(fsName))
  {
    free(vsName);
    free(fsName);
    return true;
  } else {
    free(vsName);
    free(fsName);
    return false;
  }
}

bool Shader::compile()
{
  GLint valid;
  if (program == 0)
  {
    program = glCreateProgram();
  }
  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &valid);
  if(valid == GL_FALSE)
  {
    printf("Vertex shader compilation failed\n");
    return false;
  }
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &valid);
  if(valid == GL_FALSE)
  {
    printf("Fragment shader compilation failed\n");
    return false;
  }
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glBindFragDataLocation(program, 0, "fragColor");
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &valid);
  if(valid == GL_TRUE)
  {
    printf("Link successful\n");
    return true;
  }
  else
  {
    printf("Link failed\n");
    return false;
  }
}
bool Shader::makeActiveShaderProgram()
{
  if (program)
  {
    glUseProgram(program);
    // TODO: check if shader is successfully used
    return true;
  } else {
    return false;
  }
}
bool Shader::validate()
{
  glValidateProgram(program);
  GLint valid;
  glGetProgramiv(program, GL_VALIDATE_STATUS, &valid);
  if (valid == GL_TRUE)
  {
    printf("Shader program is valid.\n");
    return true;
  } else {
    printf("Shader program is not valid.\n");
    printf("Reason: \n");
    GLint length;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &length);
    char *infoString = (char *)malloc(length);
    glGetProgramInfoLog(program, length, 0, infoString);
    printf("%s\n", infoString);
    free(infoString);
    return false;
  }
}

void Shader::getLastError(char *& err)
{
  //TODO
} 

GLuint Shader::loadShader(const char * const filename, GLenum shaderType)
{
  char *shaderSource; // String containing shader source code
  long fileSize; // Number of characters in shaderSource
  FILE *shaderFile = fopen(filename, "r"); // Shader file reference

  // Get length of file
  fseek(shaderFile, 0, SEEK_END);
  fileSize = ftell(shaderFile);
  rewind(shaderFile);

  // Read file contents into memory
  shaderSource = (char*) malloc(sizeof(char) * fileSize + 1);
  shaderSource[fileSize] = '\0';
  fread(shaderSource, sizeof(char) * fileSize, fileSize, shaderFile);
  fclose(shaderFile);

  // Debug output
  #ifdef DEBUG
  printf("******* Shader source *******\n");
  printf("%s\n", shaderSource);
  printf("*****************************\n");
  #endif
  

  // Create shader object and set its source
  GLuint shaderHandle = glCreateShader(shaderType);
  glShaderSource(shaderHandle, 1, (const GLchar **)&shaderSource, (const GLint *)&fileSize);

  // Cleanup
  free(shaderSource);

  return shaderHandle;
}

