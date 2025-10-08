#include <BufferObject.hpp>

namespace libgl
{

BufferObjectBase::BufferObjectBase(BufferTarget target) : m_target(target)
{
	glGenBuffers(1, &m_buffer);
	checkGl();
}

BufferObjectBase::BufferObjectBase(BufferObjectBase&& other) noexcept
{
	std::swap(m_buffer, other.m_buffer);
	std::swap(m_target, other.m_target);
}

BufferObjectBase::~BufferObjectBase() noexcept
{
	if (m_buffer != kEmptyHandle)
	{
		glDeleteBuffers(1, &m_buffer);
		checkGl();
	}
}

BufferObjectBase& BufferObjectBase::operator=(BufferObjectBase&& other) noexcept
{
	if (&other != this)
	{
		std::swap(m_buffer, other.m_buffer);
		std::swap(m_target, other.m_target);
	}

	return *this;
}

void BufferObjectBase::validateBinding(BufferObjectBase* self) const
{
	assert(this == self);
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

bool BufferObjectBase::supportsMapBufferRange() noexcept
{
	return glMapBufferRange != nullptr;
}

}
