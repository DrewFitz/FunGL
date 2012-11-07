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
#include "TextureUnit.h"
#include "RenderInfo.h"

// Boilerplate hiding
void initializeOpenGL();
void renderLoop();

void initializeOpenGL()
{
	RenderInfo::width = 1280;
	RenderInfo::height = 720;
	// OpenGL initialization
	glfwInit();
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwOpenWindow(RenderInfo::getWidth(), RenderInfo::getHeight(), 8, 8, 8, 8, 24, 8, GLFW_WINDOW);
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
	using namespace std::chrono;
	// Drawable geometry objects
	Torus torus;
	FullscreenQuad quad;
	//Model model("testModel");

	Printer timePrinter, fpsPrinter;
	timePrinter.setScale(1.0f);
	timePrinter.setPosition(0, 12);
	timePrinter.setScreenDimensions(RenderInfo::getWidth(), RenderInfo::getHeight());
	fpsPrinter.setScale(2.0f);
	fpsPrinter.setScreenDimensions(RenderInfo::getWidth(), RenderInfo::getHeight());

	Drawable* drawables[3];
	drawables[0] = &torus;
	drawables[1] = &quad;
	//drawables[2] = &model;

	FramebufferObject FBO(RenderInfo::getWidth() / 2, RenderInfo::getHeight() / 2);
	quad.shader.makeActiveShaderProgram();
	int fboTextureUnit = TextureUnit::getNextFreeTextureUnit();
	printf("fbo texture unit %d\n", fboTextureUnit);
	FBO.bindToTextureUnit(fboTextureUnit);
	glUniform1i(quad.shader.getUniformLocation("tex0"), fboTextureUnit);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	torus.shader.makeActiveShaderProgram();
	GLuint specularPowerLocation = torus.shader.getUniformLocation("specularPower");
	glUseProgram(0);

	///////////////

	bool printFlag = false;
	RunLoop fpsLoop([&printFlag]{
		std::this_thread::sleep_for(seconds(1));
		printFlag = true;
	});

	bool drawUI = true;
	// used for mouse input
	int totalMouseX = 0,
		totalMouseY = 0;

	// Used for runtime logging
	float thisFrameTime = 0.0f, 
		  totalTime = 0.0f;
	time_point<steady_clock> start, 
							 frameStart, 
							 frameEnd;

	start = frameStart = frameEnd = steady_clock::now();

	int specularPower = 1;
	bool spaceIsDown = false;

	char timeText[255], fpsText[255];
	sprintf(timeText, "start");
	sprintf(fpsText, "start");

	///////////////

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
		} else {
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
			glViewport(0, 0, RenderInfo::getWidth(), RenderInfo::getHeight());
			glClearColor(1.0, 1.0, 1.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			drawables[1]->draw();

		}

		if (printFlag)
		{
			snprintf(fpsText, 255, "FPS %d", int(floor(1 / thisFrameTime)));
			printFlag = false;
		}
		fpsPrinter.print(fpsText);

		snprintf(timeText, 255, "Time %02d:%02d", int(floor(totalTime / 60)), int(totalTime) % 60);
		timePrinter.print(timeText);

		glfwSwapBuffers();

		// Now the frame is done being drawn and displayed
		frameEnd = steady_clock::now();
		totalTime     = (float)duration_cast<nanoseconds>(frameEnd-start).count()      / std::nano::den;
		thisFrameTime = (float)duration_cast<nanoseconds>(frameEnd-frameStart).count() / std::nano::den;
		frameStart = frameEnd;

	}

	fpsLoop.stop();
}

int main(int argc, char* argv[])
{
	initializeOpenGL();
	// Drop into the main render loop
	renderLoop();

	glfwTerminate();
	return 0;
}
