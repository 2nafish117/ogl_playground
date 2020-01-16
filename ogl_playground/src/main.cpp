#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "GLError.h"

#include "Mesh.h"
#include "ShaderProgram.h"
#include "Time.h"

void OnFrameResize(GLFWwindow* window, int width, int height);

using namespace tf;

// Geometry data
#include "geometry/cube_indexed_textured_normals.inc"

// model loading sonar systems assimp
// https://www.youtube.com/watch?v=yQx_pMsYqzU

int main(void)
{
	// Global Data
	GLFWwindow* window;
	int windowWidth = 800;
	int windowHeight = 600;
	const char* windowText = "Hello Weebs";

	// Startup
	{
		if (!glfwInit())
			return -1;

		// Legacy OpenGl Doesnt Work with this enabled
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
			glfwTerminate();
			return -1;
		}

		std::cout << "OpenGL Version  : " << glGetString(GL_VERSION) << "\n";
		std::cout << "OpenGL Vendor	: " << glGetString(GL_VENDOR) << "\n";
		std::cout << "OpenGL Renderer : " << glGetString(GL_RENDERER) << "\n";
		std::cout << "OpenGL Shader Language Version : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

		GLFWframebuffersizefun(frameBufferResizeHandler);

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(tf::OnGlError, nullptr);
	}


	{
		// Local Data
		float verts[] = {
			// position			// texCoord		// normal
			0.5f, 0.0f, 0.0f,	0.5f, 0.0f,		0.5f, 0.0f, 0.0f,
			0.0f, 0.5f, 0.0f,	0.5f, 0.0f,		0.5f, 0.0f, 0.0f,
			-0.5f, 0.0f, 0.0f,	0.5f, 0.0f,		0.5f, 0.0f, 0.0f,
		};

		u32 inds[] = {
			0, 1, 2
		};

		float positions[] = {
			1.0f, 0.0f, 0.0f,
			1.0f, 0.5f, 0.0f,
			-1.0f, 0.0f, 0.0f,
		};
		float texCoords[] = {
			0.5f, 0.0f,
			0.5f, 0.0f,
			0.5f, 0.0f,
		};
		float normals[] = {
			0.5f, 0.0f, 0.0f,
			0.5f, 0.0f, 0.0f,
			0.5f, 0.0f, 0.0f,
		};

		SOAVertex soa_vert = {
			3, 3,
			// position					
			(glm::vec3*) positions,
			// texCoord
			(glm::vec2*) texCoords,
			// normal
			(glm::vec3*) normals,
			// indices
			(u32*) inds
		};

		AOSMesh mesh;
		mesh.loadRawData((Vertex*) verts, 3, inds, 3);
		SOAMesh mesh2;
		mesh2.loadRawData(&soa_vert);

		ShaderProgram shader;
		shader.load("res/shaders/flat_texture_shader.vert", "res/shaders/flat_texture_shader.frag");
		shader.bind();

		// Initialization
		glfwSetFramebufferSizeCallback(window, OnFrameResize);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		while (!glfwWindowShouldClose(window))
		{
			glClear(GL_COLOR_BUFFER_BIT);
			Time::update();

			//mesh.draw();
			mesh2.draw();

			glfwSwapBuffers(window);

			glfwPollEvents();
		}

		mesh.unload();
		mesh2.unload();
		shader.unload();

		glfwTerminate();
	}

	return 0;
}

void OnFrameResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
