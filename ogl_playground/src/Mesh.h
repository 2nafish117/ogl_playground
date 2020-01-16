#pragma once

#include <glm/glm.hpp>
#include "BlowTypes.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace tf
{
	using namespace glm;

	enum AttributeType
	{
		PositionAttr,
		TextureAttr,
		NormalAttr,
		IndexAttr,
		NumAttr
	};

	enum BufferType
	{
		VertexBuffer,
		IndexBuffer,
		NumBuffers
	};

	struct SOAVertex
	{
		u32 numVertices;
		u32 numIndices;
		vec3* positions;
		vec2* texCoords;
		vec3* normals;
		u32* indices;
	};

	struct SOAMesh
	{
		u32 vao;
		u32 attributes[NumAttr];
		u32 numIndices;

		void loadObjFile(const char* path);
		void loadRawData(SOAVertex* vertices);
		void _addAttr(void* buff, u32 size,
			u32 components_per_attr,
			GLenum type, AttributeType attr,
			GLenum buffer_target_type = GL_ARRAY_BUFFER);
		void draw();
		void unload();
	};

	struct AOSVertex
	{
		vec3 position;
		vec2 texCoord;
		vec3 normal;
	};

	struct AOSMesh
	{
		u32 vao;
		u32 buffers[NumBuffers];
		u32 numIndices;

		void loadObjFile(const char* path);
		void loadRawData(AOSVertex* vertices, u32 numVertices, u32* indices, u32 _numIndices);
		void _addAttr(AttributeType attr, u32 components_per_attr, u32 stride, u32 offset);
		void draw();
		void unload();
	};

	// typedef to whichever version you need and use it
	typedef AOSVertex Vertex;
	typedef AOSMesh Mesh;
}