#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data,
                        unsigned int count) :
	m_count(count)
{
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	GLCall(glGenBuffers(1, &m_rendererID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

/// <summary>
/// deletes the buffer
/// the 1 indicates that we only ha e one buffer that we actually want to delete
/// </summary>
IndexBuffer::IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_rendererID));
}

/// <summary>
/// binds the buffer
/// </summary>
void IndexBuffer::bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));
}

/// <summary>
/// setting it to 0 unbinds the buffer
/// </summary>
void IndexBuffer::unBind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
