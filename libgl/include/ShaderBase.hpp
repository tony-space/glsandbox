#pragma once

#include <opengl.hpp>

#include <string>

namespace libgl
{

class ShaderBase
{
public:
	explicit ShaderBase(GLenum type);
	ShaderBase(const ShaderBase&) = delete;
	ShaderBase(ShaderBase&&) noexcept;
	~ShaderBase() noexcept;

	ShaderBase& operator= (const ShaderBase&) = delete;
	ShaderBase& operator= (ShaderBase&&) noexcept;

	const std::string& compilationLog() const noexcept { return m_log; }
	GLuint nativeHandle() const noexcept { return m_shaderId; }

protected:
	[[nodiscard]] bool compile(const std::string& src);
private:
	constexpr static GLuint kInvalidShaderId = (std::numeric_limits<GLuint>::max)();
	GLuint m_shaderId { kInvalidShaderId };
	std::string m_log;
};

template<GLenum kType>
class ShaderImpl : public ShaderBase
{
public:
	ShaderImpl(const std::string& code);
	
	ShaderImpl(const ShaderImpl&) = delete;
	ShaderImpl(ShaderImpl&&) noexcept = default;
	
	ShaderImpl& operator=(const ShaderImpl&) = delete;
	ShaderImpl& operator=(ShaderImpl&&) noexcept = default;
};

template<GLenum kType>
ShaderImpl<kType>::ShaderImpl(const std::string& code) : ShaderBase(kType)
{
	if (!compile(code))
	{
		throw std::invalid_argument(compilationLog());
	}
}

using VertexShader = ShaderImpl<GL_VERTEX_SHADER>;
using FragmentShader = ShaderImpl<GL_FRAGMENT_SHADER>;

}
