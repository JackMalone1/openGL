#pragma once

#include <vector>
#include <GL/glew.h>
#include "Renderer.h"

struct VerrtexBufferElement
{
	unsigned int type;
	unsigned int count;	
	unsigned char normalised;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT :
			return 4;
			break;
		case GL_UNSIGNED_INT :
			return 4;
			break;
		case GL_UNSIGNED_BYTE :
			return 1;
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VerrtexBufferElement> m_Elements;
	unsigned int m_stride;
public:
	VertexBufferLayout() :
		m_stride(0){}

	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_stride += VerrtexBufferElement::GetSizeOfType(GL_FLOAT) * count;
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_stride += VerrtexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_stride += VerrtexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
	}

	inline const std::vector<VerrtexBufferElement> GetElements() const& { return m_Elements; }
	inline unsigned int GetStride() const { return m_stride; }
};
