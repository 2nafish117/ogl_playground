#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

void OnFrameResize(GLFWwindow* window, int width, int height);

int main(void)
{
	// Data
	GLFWwindow* window;
	int windowWidth = 800;
	int windowHeight = 600;
	const char* windowText = "Hello Weebs";

	// Startup
	{
		if (!glfwInit())
			return -1;

		// Legacy OpenGl Doesnt Work with this enabled
		// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(windowWidth, windowHeight, windowText, NULL, NULL);
		if (!window)
		{
			glfwTerminate();
			return -1;
		}

		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "[Error] Failed to initialize GLAD\n";
			return -1;
		}
	}


	// Initialization
	glfwSetFramebufferSizeCallback(window, OnFrameResize);
	//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_TRIANGLES);
		glVertex2f(0.5, 0);
		glVertex2f(-0.5, 0);
		glVertex2f(0, 0.5);
		glEnd();

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void OnFrameResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
