#include <BufferObject.hpp>

namespace libgl
{

BufferObjectBase::BufferObjectBase(BufferTarget target) : m_target(target)
{
	glGenBuffers(1, &m_buffer);
	checkGl();
}

BufferObjectBase::~BufferObjectBase() noexcept
{
	if (m_buffer != kEmptyHandle)
	{
		glDeleteBuffers(1, &m_buffer);
		checkGl();
	}
}

void BufferObjectBase::bind()
{
	GLenum target = static_cast<GLenum>(m_target);
	glBindBuffer(target, m_buffer);
	checkGl();
}

void BufferObjectBase::unbind()
{
	GLenum target = static_cast<GLenum>(m_target);
	glBindBuffer(target, 0);
	checkGl();
}

}
