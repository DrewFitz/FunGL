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
	Shader textShader;
	textShader.loadShaderPairByName("text");
	textShader.compile();
	textShader.makeActiveShaderProgram();

	char* textBuffer = new char[strlen(text)];
	for (int i = 0; i < strlen(text); ++i)
	{
		char letter = text[i];
		if (letter > 'a' && letter < 'z')
			textBuffer[i] = letter - 'a';
		else if (letter > 'A' && letter < 'Z')
			textBuffer[i] = letter - 'A';
		else if (letter > '0' && letter < '9')
			textBuffer[i] = letter - '0' + (2 * (64 / 5));
	}

	GLuint textTexture;
	glGenTextures(1, &textTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_1D, textTexture);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_R8, strlen(text), 0, GL_RED, GL_BYTE, textBuffer);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_1D, 0);

	GLuint textureAtlas;
	glGenTextures(1, &textureAtlas);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textureAtlas);
	if (glfwLoadTexture2D("resources/images/letters.tga", 0) == GL_TRUE)
		printf("Texture loaded\n");
	else	
		printf("Texture load failed\n");

	glUniform1i(textShader.getUniformLocation("textTexture"), 2);
	glUniform1i(textShader.getUniformLocation("textAtlas"), 3);

	glUniform2i(textShader.getUniformLocation("atlasDimensions"), 64, 64);
	glUniform2i(textShader.getUniformLocation("atlasCellDimensions"), 5, 5);

	glUniform2f(textShader.getUniformLocation("drawLocation"), 500.0f, 500.0f);
	glUniform2f(textShader.getUniformLocation("screenDimensions"), 1920.0f, 1080.0f);
	glUniform1f(textShader.getUniformLocation("scale"), 10.0f);



	GLfloat vertices[12] = {0.0, 0.0, 0.0,
							1.0, 0.0, 0.0,
							0.0, 1.0, 0.0,
							1.0, 1.0, 0.0};

	GLuint VAO, vbo;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &vbo);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 12*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(textShader.getAttributeLocation("vVertex"));
	glVertexAttribPointer(textShader.getAttributeLocation("vVertex"), 3, GL_FLOAT, GL_FALSE, 0, 0);

	textShader.validate();
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 12, strlen(text));

	delete[] textBuffer;
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

	printf("OpenGL version: %s\n", (char*)glGetString(GL_VERSION));
	printf("GLSL version:   %s\n", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

	// Don't vsync
	glfwSwapInterval(0);
}

void renderLoop()
{
	bool drawUI = true;
	printf("UI Mode: %s\n", drawUI ? "on" : "off");
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

	char* text = new char[256];
	sprintf(text, "hello");
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	gprint(text);
	glfwSwapBuffers();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	gprint(text);

	while (!glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED))
	{
		/*
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
				printf("UI Mode: %s\n", drawUI ? "on" : "off");
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
			drawables[2]->draw();
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

	*/	


		//sprintf(text, "FPS %d", int(floor(thisFrame)));
		//gprint(text);

		glfwSwapBuffers();

		// Now the frame is done being drawn and displayed
		frameEnd = std::chrono::steady_clock::now();
		totalTime = (float)std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd-start).count() / std::milli::den;
		thisFrame = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(frameEnd-frameStart).count() / std::nano::den;
		if (spaceIsDown)
		{
			printf("FPS: %d\n", int(1/thisFrame));
			
		}
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
