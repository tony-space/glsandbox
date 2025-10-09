#include <VertexArrayObject.hpp>

namespace libgl
{

VertexArrayObject::VertexArrayObject()
{
	glGenVertexArrays(1, &m_vao);
	checkGl();
}

VertexArrayObject::VertexArrayObject(VertexArrayObject&& other) noexcept
{
	std::swap(other.m_vao, m_vao);
}

VertexArrayObject::~VertexArrayObject()
{
	if (m_vao != kEmptyHandle)
	{
		glDeleteVertexArrays(1, &m_vao);
		checkGl();
	}
}

VertexArrayObject& VertexArrayObject::operator= (VertexArrayObject&& other) noexcept
{
	if (&other != this)
	{
		std::swap(other.m_vao, m_vao);
	}

	return *this;
}

void VertexArrayObject::bind()
{
	glBindVertexArray(m_vao);
	checkGl();
}

void VertexArrayObject::unbind()
{
	glBindVertexArray(0);
	checkGl();

}

void VertexArrayObject::setVertexAttribute(GLint shaderAttributeLocation, const VertexAttribute& info)
{
	glEnableVertexAttribArray(shaderAttributeLocation);
	checkGl();

	glVertexAttribPointer(shaderAttributeLocation, info.components, static_cast<GLenum>(info.type), info.normalized, info.bytesStride, reinterpret_cast<const void*>(info.byteOffset));
	checkGl();

	glVertexAttribDivisor(shaderAttributeLocation, info.divisor);
	checkGl();
}


void VertexArrayObject::disableAttribute(GLint shaderAttributeLocation)
{
	glDisableVertexAttribArray(shaderAttributeLocation);
	checkGl();
}


}
