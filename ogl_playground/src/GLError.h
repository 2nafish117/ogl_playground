#pragma once

#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace TF
{
	void GLAPIENTRY
		MessageCallback(GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam);
}