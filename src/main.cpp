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
#include "Printer.h"

int screenWidth  = 1920,
	screenHeight = 1080;

// Boilerplate hiding
void initializeOpenGL();
void renderLoop();

// TODO
/*
 * load targa manually
 * move gprint() to class
 *  '-> make text area class
 * make ui managing class
 * make gprint more robust
 * build up material manager + material shaders
 * 
 *
 */

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
	// Drawable geometry objects
	Torus torus;
	FullscreenQuad quad;
	//Model model("testModel");

	Printer printer;

	Drawable* drawables[3];
	drawables[0] = &torus;
	drawables[1] = &quad;
	//drawables[2] = &model;

	FramebufferObject FBO(960, 540);
	quad.shader.makeActiveShaderProgram();
	FBO.bindToTextureUnit(1);
	glUniform1i(quad.shader.getUniformLocation("tex0"), 1);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	torus.shader.makeActiveShaderProgram();
	GLuint specularPowerLocation = torus.shader.getUniformLocation("specularPower");
	glUseProgram(0);

	/////////////

	bool printFlag = false;
	RunLoop fpsLoop([&printFlag]{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		printFlag = true;
	});

	bool drawUI = true;
	// used for mouse input
	int totalMouseX = 0,
		totalMouseY = 0;

	// Used for runtime logging
	float thisFrame = 0.0f, 
		  totalTime = 0.0f;
	std::chrono::time_point<std::chrono::steady_clock> start, 
													   frameStart, 
													   frameEnd;

	start = frameStart = frameEnd = std::chrono::steady_clock::now();

	int specularPower = 1;
	bool spaceIsDown = false;

	char text[255];
	sprintf(text, "start");

	while (!glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED))
	{
		glfwPollEvents();
		glfwGetMousePos(&totalMouseX, &totalMouseY);
		if(glfwGetKey( GLFW_KEY_UP ))
		{
			++specularPower;
			torus.shader.makeActiveShaderProgram();
			glUniform1i(specularPowerLocation, specularPower);
		} else if(glfwGetKey( GLFW_KEY_DOWN )) {
			--specularPower;
			torus.shader.makeActiveShaderProgram();
			glUniform1i(specularPowerLocation, specularPower);
		}

		if (glfwGetKey( GLFW_KEY_SPACE ))
		{
			if (!spaceIsDown) {
				drawUI = !drawUI;
				spaceIsDown = true;
			}
		} else if (spaceIsDown) {
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

		if (printFlag)
		{
			sprintf(text, "FPS %d", int(floor(1 / thisFrame)));
			printFlag = false;
		}

		printer.print(text);

		glfwSwapBuffers();

		// Now the frame is done being drawn and displayed
		frameEnd = std::chrono::steady_clock::now();
		totalTime = (float)std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd-start).count() / std::milli::den;
		thisFrame = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(frameEnd-frameStart).count() / std::nano::den;
		frameStart = frameEnd;

	}

	fpsLoop.stop();

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
