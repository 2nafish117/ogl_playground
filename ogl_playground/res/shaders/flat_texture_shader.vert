#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec2 texCoord;

// uniform mat4 uTransform;
// uniform mat4 uViewProjection;

void main()
{
	texCoord = aTexCoord;
	// vec4 transformedPos = uViewProjection * uTransform * vec4(aPosition, 1.0f);
	gl_Position = vec4(aPosition, 1.0f);
}