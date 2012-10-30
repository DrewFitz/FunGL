#include "Shader.h"

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
  const unsigned int length = strlen("resources/shaders/") + strlen(name) + 3;
  char* vsName = new char[length];
  char* fsName = new char[length];
  sprintf(vsName, "resources/shaders/%s%s", name, ".vs");
  sprintf(fsName, "resources/shaders/%s%s", name, ".fs");
  printf("Status for shader \"%s\": ", name);
  if(loadVertexShader(vsName) && loadFragmentShader(fsName))
  {
    delete[] vsName;
    delete[] fsName;
    return true;
  } else {
    delete[] vsName;
    delete[] fsName;
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
  makeActiveShaderProgram();
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

GLuint Shader::loadShader(const char * const filename, GLenum shaderType)
{
  char *shaderSource; // String containing shader source code
  long fileSize; // Number of characters in shaderSource
  FILE *shaderFile = fopen(filename, "r"); // Shader file reference
  if (shaderFile == NULL)
  {
    return 0;
  }

  // Get length of file
  fseek(shaderFile, 0, SEEK_END);
  fileSize = ftell(shaderFile);
  rewind(shaderFile);

  // Read file contents into memory
  shaderSource = (char*) malloc(sizeof(char) * fileSize + 1);
  shaderSource[fileSize] = '\0';
  fread(shaderSource, sizeof(char) * fileSize, fileSize, shaderFile);
  fclose(shaderFile);

  // Create shader object and set its source
  GLuint shaderHandle = glCreateShader(shaderType);
  glShaderSource(shaderHandle, 1, (const GLchar **)&shaderSource, (const GLint *)&fileSize);

  // Cleanup
  free(shaderSource);

  return shaderHandle;
}

