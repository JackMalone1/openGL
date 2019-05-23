#include "VertexBuffer.h"
#include "Renderer.h"

/// <summary>
/// 
/// </summary>
/// <param name="data"></param>
/// <param name="size">size in bytes</param>
VertexBuffer::VertexBuffer(const void * data, unsigned int size)
{
	GLCall(glGenBuffers(1, &m_rendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

/// <summary>
/// deletes the buffer
/// the 1 indicates that we only ha e one buffer that we actually want to delete
/// </summary>
VertexBuffer::VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_rendererID));
}

/// <summary>
/// binds the buffer
/// </summary>
void VertexBuffer::bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
}

/// <summary>
/// setting it to 0 unbinds the buffer
/// </summary>
void VertexBuffer::unBind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
