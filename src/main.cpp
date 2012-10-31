#include <chrono>
#include <thread>

#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include <GL/glfw.h>

#include "FrameCounter.h"
#include "RunLoop.h"

#include "FramebufferObject.h"
#include "Torus.h"
#include "FullscreenQuad.h"
#include "Model.h"

int screenWidth  = 1920,
	screenHeight = 1080;

// Boilerplate hiding
void initializeOpenGL();
void renderLoop();
void gprint(const char* text);


void gprint(const char* text)
{
	static Shader textShader;
	static bool setup = true;
	static GLuint textTexture, textureAtlas;
	static GLuint VAO;
	if (setup)
	{
		textShader.loadShaderPairByName("text");
		textShader.compile();
		textShader.makeActiveShaderProgram();

		glGenTextures(1, &textTexture);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_1D, textTexture);
		glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		unsigned char* dummy = new unsigned char[255];
		memset(dummy, 0, sizeof(unsigned char) * 255);
		glTexImage1D(GL_TEXTURE_1D, 0, GL_RED, 255, 0, GL_RED, GL_UNSIGNED_BYTE, dummy);
		delete[] dummy;

		glGenTextures(1, &textureAtlas);
		glActiveTexture(GL_TEXTURE3);
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

		glUniform1i(textShader.getUniformLocation("textTexture"), 2);
		glUniform1i(textShader.getUniformLocation("textAtlas"), 3);

		glUniform2i(textShader.getUniformLocation("atlasDimensions"), 64, 64);
		glUniform2i(textShader.getUniformLocation("atlasCellDimensions"), 5, 6);

		glUniform2f(textShader.getUniformLocation("drawLocation"), 000.0f, 000.0f);
		glUniform2f(textShader.getUniformLocation("screenDimensions"), 1920.0f, 1080.0f);
		glUniform1f(textShader.getUniformLocation("scale"), 50.0f);



		GLfloat vertices[12] = {0.0, 0.0, 0.0,
								1.0, 0.0, 0.0,
								0.0, 1.0, 0.0,
								1.0, 1.0, 0.0};

		GLuint vbo;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &vbo);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 12*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(textShader.getAttributeLocation("vVertex"));
		glVertexAttribPointer(textShader.getAttributeLocation("vVertex"), 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		setup = false;
	}
	unsigned char* textBuffer = new unsigned char[strlen(text)];
	for (int i = 0; i < strlen(text); ++i)
	{
		char letter = text[i];
		if (letter >= 'a' && letter <= 'z')
			textBuffer[i] = letter - 'a';
		else if (letter >= 'A' && letter <= 'Z')
			textBuffer[i] = letter - 'A';
		else if (letter >= '0' && letter <= '9')
			textBuffer[i] = letter - '0' + 36;
	}
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_1D, textTexture);
	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, strlen(text), GL_RED, GL_UNSIGNED_BYTE, textBuffer);
	delete[] textBuffer;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_1D, 0);

	glBindVertexArray(VAO);
	textShader.makeActiveShaderProgram();
	glUniform1i(textShader.getUniformLocation("stringLength"), strlen(text));
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 12, strlen(text));
	glBindVertexArray(0);

}

void initializeOpenGL()
{
	// OpenGL initialization
	glfwInit();
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwOpenWindow(screenWidth, screenHeight, 8, 8, 8, 8, 24, 8, GLFW_WINDOW);
	glfwDisable(GLFW_MOUSE_CURSOR);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glBlendEquation(GL_MAX);

	printf("OpenGL version: %s\n", (char*)glGetString(GL_VERSION));
	printf("GLSL version:   %s\n", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

	// Don't vsync
	glfwSwapInterval(0);
}

void renderLoop()
{
	bool drawUI = true;
	// used for mouse input
	int totalMouseX = 0,
		totalMouseY = 0;

	// Used for runtime logging
	float thisFrame = 0.0f, 
		  totalTime = 0.0f;
	FrameCounter FC;
	std::chrono::time_point<std::chrono::steady_clock> start, 
													   frameStart, 
													   frameEnd;

	// Drawable geometry objects
	Torus torus;
	FullscreenQuad quad;
	Model model("testModel");

	Drawable* drawables[3];
	drawables[0] = &torus;
	drawables[1] = &quad;
	drawables[2] = &model;

	FramebufferObject FBO(960, 540);
	quad.shader.makeActiveShaderProgram();
	FBO.bindToTextureUnit(1);
	glUniform1i(quad.shader.getUniformLocation("tex0"), 1);

	/*
	RunLoop fpsLoop([&FC]{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		FC.print();
	});
	*/

	start = frameStart = frameEnd = std::chrono::steady_clock::now();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	torus.shader.makeActiveShaderProgram();
	GLuint specularPowerLocation = torus.shader.getUniformLocation("specularPower");
	glUseProgram(0);
	int specularPower = 1;
	bool spaceIsDown = false;

	char text[255];
	sprintf(text, "start");

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	int gprintCount = 0;
	while (!glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED))
	{
		glfwPollEvents();
		glfwGetMousePos(&totalMouseX, &totalMouseY);
		if(glfwGetKey( GLFW_KEY_UP ))
		{
			++specularPower;
			torus.shader.makeActiveShaderProgram();
			glUniform1i(specularPowerLocation, specularPower);
		} 
		else if(glfwGetKey( GLFW_KEY_DOWN ))
		{
			--specularPower;
			torus.shader.makeActiveShaderProgram();
			glUniform1i(specularPowerLocation, specularPower);
		}

		if (glfwGetKey( GLFW_KEY_SPACE ))
		{
			if (!spaceIsDown)
			{
				drawUI = !drawUI;
				spaceIsDown = true;
			}
		} 
		else if (spaceIsDown) 
		{
			spaceIsDown = false;
		}

		quad.shader.makeActiveShaderProgram();
		glUniform1f(quad.shader.getUniformLocation("time"), totalTime);
		torus.update(totalMouseX, totalMouseY);

		if (!drawUI)
		{
			glClearColor(1.0, 0.0, 1.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			drawables[0]->draw();
			//drawables[2]->draw();
		} else {

			// ********** Pass 1
			FBO.makeActiveFramebuffer();
			glClearColor(1.0, 0.0, 1.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			drawables[0]->draw();

			// ********** Pass 2
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, screenWidth, screenHeight);
			glClearColor(1.0, 1.0, 1.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			drawables[1]->draw();

		}



		if (gprintCount > 100)
		{
			sprintf(text, "FPS %d", int(floor(1 / thisFrame)));
			gprintCount = 0;
		}
		++gprintCount;

		glEnable(GL_BLEND);
		gprint(text);
		glDisable(GL_BLEND);

		glfwSwapBuffers();

		// Now the frame is done being drawn and displayed
		frameEnd = std::chrono::steady_clock::now();
		totalTime = (float)std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd-start).count() / std::milli::den;
		thisFrame = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(frameEnd-frameStart).count() / std::nano::den;
		frameStart = frameEnd;

		//++FC;
	}

	//fpsLoop.stop();

	printf("Last frame length:	  %f\n", thisFrame);
	printf("Total time ticked:	  %f\n", totalTime);
	printf("Final specular power: %d\n", specularPower);

}

int main(int argc, char* argv[])
{
	initializeOpenGL();
	// Drop into the main render loop
	renderLoop();

	glfwTerminate();
	return 0;
}
