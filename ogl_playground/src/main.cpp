#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "GLError.h"

#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Time.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/quaternion.hpp>

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
	int windowWidth = 1280;
	int windowHeight = 720;
	const char* windowText = "Hello Weebs";

	static f64 xpos_now, ypos_now;
	static f64 xpos_prev, ypos_prev;

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
			0.5f, 0.0f, 0.0f,	1.0f, 0.0f,		0.5f, 0.0f, 0.0f,
			0.0f, 0.7f, 0.0f,	0.5f, 1.0f,		0.5f, 0.0f, 0.0f,
			-0.5f, 0.0f, 0.0f,	0.0f, 0.0f,		0.5f, 0.0f, 0.0f,
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

		EulerFpsCamera camera;
		
		camera.init();
		
		camera.aspect = (f32) windowWidth / windowHeight;
		camera.speed = 10.0f;


		AOSMesh mesh;
		mesh.loadRawData((Vertex*) verts, 3, inds, 3);
		Texture tex;
		tex.load("res/textures/wood_crate_texture.png");

		SOAMesh mesh2;
		mesh2.loadRawData(&soa_vert);

		ShaderProgram shader;
		shader.load("res/shaders/flat_texture_shader.vert", "res/shaders/flat_texture_shader.frag");

		// Initialization
		glfwSetFramebufferSizeCallback(window, OnFrameResize);
		glfwSetCursorPos(window, 0.0, 0.0);
		// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		while (!glfwWindowShouldClose(window))
		{
			glClear(GL_COLOR_BUFFER_BIT);
			Time::update();

			{
				// Input
				xpos_prev = xpos_now;
				ypos_prev = ypos_now;
				glfwGetCursorPos(window, &xpos_now, &ypos_now);
				//std::cout << xpos_prev << ", " << ypos_prev << " -> " << xpos_now << ", " << ypos_now << "\n";
				camera.handleMouseMove(xpos_prev, ypos_prev, xpos_now, ypos_now);

				if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
					glfwSetWindowShouldClose(window, GLFW_TRUE);

				if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
				{

				}
			}

			// glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(80.0f), glm::vec3(0, 1, 0));
			// glm::mat4 view = glm::lookAt(glm::vec3(0, 0, -5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
			// glm::mat4 proj = glm::perspective(glm::radians(60.0f), (float) windowWidth / windowHeight, 0.1f, 100.0f);

			glm::vec3 rotAxis = glm::vec3(1, 1, 1);
			rotAxis = glm::normalize(rotAxis);
			glm::vec3 scale(1, 1, 1);
			glm::quat orientation = glm::angleAxis((float) 0.0f, rotAxis);
			glm::vec3 position(0, 0, 0);

			glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scale);
			glm::mat4 orientationMat = glm::toMat4(orientation);
			glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), position);

			glm::mat4 model = translationMat * orientationMat * scaleMat;
			glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
			glm::mat4 proj = glm::perspective(glm::radians(60.0f), (float) windowWidth / windowHeight, 0.1f, 100.0f);

			//glm::mat4 viewProj = proj * view;
			glm::mat4 viewProj = camera.getViewProj();

			shader.bind();
			shader.set1i("uTexture", 0);
			shader.setMat4fv("uTransform", glm::value_ptr(model));
			shader.setMat4fv("uViewProjection", glm::value_ptr(viewProj));

			tex.bind(0);
			mesh.draw();
			//mesh2.draw();

			glfwSwapBuffers(window);

			glfwPollEvents();
		}

		mesh.unload();
		mesh2.unload();
		tex.unload();
		shader.unload();

		glfwTerminate();
	}

	return 0;
}

void OnFrameResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
