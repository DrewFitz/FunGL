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

int screenWidth  = 1920,
	screenHeight = 1080;

// Boilerplate hiding
void initializeOpenGL();
void renderLoop();

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
	Drawable *drawables[2];
	drawables[0] = &torus;
	drawables[1] = &quad;
	FramebufferObject FBO;
	FBO.bindToTextureUnit(1);
	glUniform1i(quad.shader.getUniformLocation("tex0"), 1);

	RunLoop fpsLoop([&FC]{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		FC.print();
	});

	start = frameStart = frameEnd = std::chrono::steady_clock::now();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	torus.shader.makeActiveShaderProgram();
	GLuint specularPowerLocation = torus.shader.getUniformLocation("specularPower");
	int specularPower = 1;
	bool spaceIsDown = false;
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
			//torus.draw();
			drawables[0]->draw();
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

		glfwSwapBuffers();

		// Now the frame is done being drawn and displayed
		frameEnd = std::chrono::steady_clock::now();
		totalTime = (float)std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd-start).count() / std::milli::den;
		thisFrame = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(frameEnd-frameStart).count() / std::nano::den;
		frameStart = frameEnd;

		++FC;
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
