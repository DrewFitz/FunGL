#include "Printer.h"

Printer::Printer() : scale(1.0f), VAO(0), vbo(0), textTexture(0), textureAtlas(0)
{
	setShader("text");
	shader.makeActiveShaderProgram();

	int textTextureUnitNumber, textureAtlasTextureUnitNumber;
	textTextureUnitNumber= TextureUnit::getNextFreeTextureUnit();
	textureAtlasTextureUnitNumber = TextureUnit::getNextFreeTextureUnit();
	printf("text texture unit:  %d\n", textTextureUnitNumber);
	printf("atlas texture unit: %d\n", textureAtlasTextureUnitNumber);
	textTextureUnit = GL_TEXTURE0 + textTextureUnitNumber;
	textureAtlasTextureUnit = GL_TEXTURE0 + textureAtlasTextureUnitNumber;

	glGenTextures(1, &textTexture);
	glActiveTexture(textTextureUnit);
	glBindTexture(GL_TEXTURE_1D, textTexture);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// zero out texture data
	unsigned char* dummy = new unsigned char[255];
	memset(dummy, 0, sizeof(unsigned char) * 255);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RED, 255, 0, GL_RED, GL_UNSIGNED_BYTE, dummy);
	delete[] dummy;

	glGenTextures(1, &textureAtlas);
	glActiveTexture(textureAtlasTextureUnit);
	glBindTexture(GL_TEXTURE_2D, textureAtlas);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	if(GL_TRUE == glfwLoadTexture2D("resources/images/letters.tga", 0))
		printf("Texture loaded\n");
	else
		printf("Texture not loaded\n");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_1D, 0);

	glUniform1i(shader.getUniformLocation("textTexture"), textTextureUnitNumber);
	glUniform1i(shader.getUniformLocation("textAtlas"), textureAtlasTextureUnitNumber);

	glUniform2i(shader.getUniformLocation("atlasDimensions"), 64, 64);
	glUniform2i(shader.getUniformLocation("atlasCellDimensions"), 5, 6);

	glUniform2f(shader.getUniformLocation("drawLocation"), 0.0f, 0.0f);
	glUniform2i(shader.getUniformLocation("screenDimensions"), 1280, 720);
	glUniform1f(shader.getUniformLocation("scale"), scale);



	GLfloat vertices[12] = {0.0, 0.0, 0.0,
							1.0, 0.0, 0.0,
							0.0, 1.0, 0.0,
							1.0, 1.0, 0.0};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &vbo);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 12*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(shader.getAttributeLocation("vVertex"));
	glVertexAttribPointer(shader.getAttributeLocation("vVertex"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


Printer::~Printer()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &VAO);
	glDeleteTextures(1, &textTexture);
	glDeleteTextures(1, &textureAtlas);
}

void Printer::setScreenDimensions(int width, int height)
{
	shader.makeActiveShaderProgram();
	glUniform2i(shader.getUniformLocation("screenDimensions"), width, height);
	glUseProgram(0);
}

void Printer::setPosition(int x, int y)
{
	shader.makeActiveShaderProgram();
	glUniform2f(shader.getUniformLocation("drawLocation"), x, y);
	glUseProgram(0);
}

void Printer::setScale(float scaleFactor)
{
	if (scaleFactor != 0.0f)
	{
		scale = scaleFactor;
		shader.makeActiveShaderProgram();
		glUniform1f(shader.getUniformLocation("scale"), scale);
		glUseProgram(0);
	}
}

void Printer::setShader(const char* shaderName)
{
	shader.loadShaderPairByName(shaderName);
	shader.compile();
}

void Printer::setTextureAtlas(const char* atlasName)
{
	glActiveTexture(textureAtlasTextureUnit);
	glBindTexture(GL_TEXTURE_2D, textureAtlas);
	glfwLoadTexture2D(atlasName, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Printer::print(const char* text)
{
	unsigned char* textBuffer = new unsigned char[strlen(text)];
	for (unsigned int i = 0; i < strlen(text); ++i)
	{
		char letter = text[i];
		if (letter >= 'a' && letter <= 'z')
			textBuffer[i] = letter - 'a';
		else if (letter >= 'A' && letter <= 'Z')
			textBuffer[i] = letter - 'A';
		else if (letter >= '0' && letter <= '9')
			textBuffer[i] = letter - '0' + 36;
		else if (letter >= '!' && letter <= ',')
			textBuffer[i] = letter - '!' + 48;
		else if (letter >= '.' && letter <= '/')
			textBuffer[i] = letter - '.' + 60;
		else if (letter >= ':' && letter <= '@')
			textBuffer[i] = letter - ':' + 72;
		else if (letter >= '[' && letter <= '`')
			textBuffer[i] = letter - '[' + 84;
		else if (letter >= '{' && letter <= '~')
			textBuffer[i] = letter - '{' + 96;
		else
			textBuffer[i] = 108;
	}
	glActiveTexture(textTextureUnit);
	glBindTexture(GL_TEXTURE_1D, textTexture);
	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, strlen(text), GL_RED, GL_UNSIGNED_BYTE, textBuffer);
	delete[] textBuffer;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_1D, 0);

	glBindVertexArray(VAO);
	shader.makeActiveShaderProgram();
	glUniform1i(shader.getUniformLocation("stringLength"), strlen(text));
	glEnable(GL_BLEND);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 12, strlen(text));
	glDisable(GL_BLEND);
	glBindVertexArray(0);
}
