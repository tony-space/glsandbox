#include <VertexArrayObject.hpp>

namespace libgl
{

VertexArrayObject::VertexArrayObject()
{
	if (glGenVertexArrays)
	{
		glGenVertexArrays(1, &m_vao);
	}
#ifdef glGenVertexArraysAPPLE
	else if (glGenVertexArraysAPPLE)
	{
		glGenVertexArraysAPPLE(1, &m_vao);
	}
#endif
	else
	{
		std::terminate();
	}
	checkGl();
}

VertexArrayObject::VertexArrayObject(VertexArrayObject&& other) noexcept
{
	std::swap(other.m_vao, m_vao);
}

VertexArrayObject::~VertexArrayObject()
{
	if (m_vao == kEmptyHandle)
		return;
	
	if (glDeleteVertexArrays)
	{
		glDeleteVertexArrays(1, &m_vao);
	}
#ifdef glDeleteVertexArraysAPPLE
	else if (glDeleteVertexArraysAPPLE)
	{
		glDeleteVertexArraysAPPLE(1, &m_vao);
	}
#endif
	else
	{
		std::terminate();
	}
	checkGl();
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
	if (glBindVertexArray)
	{
		glBindVertexArray(m_vao);
	}
#ifdef glBindVertexArrayAPPLE
	else if (glBindVertexArrayAPPLE)
	{
		glBindVertexArrayAPPLE(m_vao);
	}
#endif
	else
	{
		std::terminate();
	}
	checkGl();
}

void VertexArrayObject::unbind()
{
	if (glBindVertexArray)
	{
		glBindVertexArray(0);
	}
#ifdef glBindVertexArrayAPPLE
	else if (glBindVertexArrayAPPLE)
	{
		glBindVertexArrayAPPLE(0);
	}
#endif
	else
	{
		std::terminate();
	}
	checkGl();
}

void VertexArrayObject::setVertexAttribute(GLint shaderAttributeLocation, const VertexAttribute& info)
{
	glEnableVertexAttribArray(shaderAttributeLocation);
	checkGl();

	glVertexAttribPointer(shaderAttributeLocation, info.components, static_cast<GLenum>(info.type), info.normalized, info.bytesStride, reinterpret_cast<const void*>(info.byteOffset));
	checkGl();

	if (glVertexAttribDivisor)
	{
		glVertexAttribDivisor(shaderAttributeLocation, info.divisor);
	}
#ifdef glVertexAttribDivisorARB
	else if (glVertexAttribDivisorARB)
	{
		glVertexAttribDivisorARB(shaderAttributeLocation, info.divisor);
	}
#endif
	else
	{
		std::terminate();
	}
	checkGl();
}


void VertexArrayObject::disableAttribute(GLint shaderAttributeLocation)
{
	glDisableVertexAttribArray(shaderAttributeLocation);
	checkGl();
}


}
