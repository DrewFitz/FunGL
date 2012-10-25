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
	glfwOpenWindow(1920, 1080, 8, 8, 8, 8, 8, 0, GLFW_WINDOW);
	glfwDisable(GLFW_MOUSE_CURSOR);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	printf("OpenGL version: %s\n", (char*)glGetString(GL_VERSION));
	printf("GLSL version:   %s\n", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

	// Don't vsync
	glfwSwapInterval(0);
}

void renderLoop()
{
	GLuint FBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Setup texture target for FBO rendering
	GLuint FBOTexture;
	glGenTextures(1, &FBOTexture);
	glBindTexture(GL_TEXTURE_2D, FBOTexture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1920, 1080, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBOTexture, 0);

	// Setup renderbuffer to hold depth
	GLuint depthRBO;
	glGenRenderbuffers(1, &depthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1920, 1080);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRBO);

	// Check if our framebuffer is properly constructed	
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch(status)
	{
		case GL_FRAMEBUFFER_COMPLETE:
			printf("Framebuffer complete!\n"); 
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			printf("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT\n"); 
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:    
			printf("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT\n"); 
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:    
			printf("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER\n"); 
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:    
			printf("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER\n"); 
			break;
		case GL_FRAMEBUFFER_UNDEFINED:
			printf("GL_FRAMEBUFFER_UNDEFINED\n"); 
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			printf("GL_FRAMEBUFFER_UNSUPPORTED\n"); 
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			printf("GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE\n"); 
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
			printf("GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS\n"); 
			break;
		case 0:
			printf("Some error occurred apparently.\n");
			break;

		default:
			printf("WTF?!\n");
			break;
	}


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, FBOTexture);





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
	glUniform1i(quad.shader.getUniformLocation("tex0"), 0);

	RunLoop fpsLoop([&FC]{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		FC.print();
	});

	start = frameStart = frameEnd = std::chrono::steady_clock::now();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
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


		quad.shader.makeActiveShaderProgram();
		glUniform1f(quad.shader.getUniformLocation("time"), totalTime);
		torus.update(totalMouseX, totalMouseY);

		// Drawing to multiple "virtual screens"
		//glViewport(0, 0, 640, 720);
		//torus.draw();
		//glViewport(640, 0, 640, 720);
		//torus.draw();
		//glViewport(0, 0, 1280, 720);

		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glClearColor(1.0, 0.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		torus.draw();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		quad.draw();

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
