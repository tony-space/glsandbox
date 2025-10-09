#include <ShaderBase.hpp>

#include <cassert>

namespace libgl
{

ShaderBase::ShaderBase(GLenum type)
{
	m_shaderId = glCreateShader(type);
	checkGl();
}

ShaderBase::ShaderBase(ShaderBase&& o) noexcept
{
	std::swap(o.m_shaderId, m_shaderId);
	std::swap(o.m_log, m_log);
}

ShaderBase::~ShaderBase() noexcept
{
	if(m_shaderId != kInvalidShaderId)
	{
		glDeleteShader(m_shaderId);
		checkGl();
	}
}

ShaderBase& ShaderBase::operator=(ShaderBase&& o) noexcept
{
	if (this != &o)
	{
		std::swap(o.m_shaderId, m_shaderId);
		std::swap(o.m_log, m_log);
	}
	
	return *this;
}

bool ShaderBase::compile(const std::string& src)
{
	assert(!src.empty());

	const GLchar* const lines[1] = { src.c_str() };
	glShaderSource(m_shaderId, 1, lines, nullptr);
	checkGl();

	glCompileShader(m_shaderId);
	checkGl();

	GLint logSize;
	glGetShaderiv(m_shaderId, GL_INFO_LOG_LENGTH, &logSize);
	checkGl();

	if (logSize)
	{
		std::vector<GLchar> rawLog(logSize, GLchar{ 0 });
		glGetShaderInfoLog(m_shaderId, logSize, nullptr, rawLog.data());
		checkGl();
		m_log = std::string(rawLog.cbegin(), rawLog.cend());
	}

	GLint compileStatus;
	glGetShaderiv(m_shaderId, GL_COMPILE_STATUS, &compileStatus);
	checkGl();
	return compileStatus == GL_TRUE;
}

}
