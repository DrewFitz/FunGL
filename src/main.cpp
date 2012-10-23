#include <chrono>
#include <thread>

#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include <GL/glfw.h>

#include "FrameCounter.h"
#include "RunLoop.h"

#include "Torus.h"
#include "FullscreenQuad.h"

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
	glfwOpenWindow(1280, 720, 8, 8, 8, 8, 8, 0, GLFW_WINDOW);
	glfwDisable(GLFW_MOUSE_CURSOR);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	printf("OpenGL version: %s\n", (char*)glGetString(GL_VERSION));
	printf("GLSL version:   %s\n", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

	// Don't vsync
	//glfwSwapInterval(0);
}

void renderLoop()
{
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

	RunLoop fpsLoop([&FC]{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		FC.print();
	});

	start = frameStart = frameEnd = std::chrono::steady_clock::now();
	
	int specularPower = 1;
	while (!glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED ))
	{
		glfwPollEvents();
		glfwGetMousePos(&totalMouseX, &totalMouseY);
		torus.shader.makeActiveShaderProgram();
		if(glfwGetKey(GLFW_KEY_UP))
		{
			++specularPower;
			glUniform1i(torus.shader.getUniformLocation("specularPower"), specularPower);
		} 
		else if(glfwGetKey(GLFW_KEY_DOWN))
		{
			--specularPower;
			glUniform1i(torus.shader.getUniformLocation("specularPower"), specularPower);
		}


		glClearColor(1.0, 0.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		torus.update(totalMouseX, totalMouseY);

		// Drawing to multiple "virtual screens"
		//glViewport(0, 0, 640, 720);
		//torus.draw();
		//glViewport(640, 0, 640, 720);
		//torus.draw();
		//glViewport(0, 0, 1280, 720);

		torus.draw();
		
		//quad.draw();

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

	return 0;
}
