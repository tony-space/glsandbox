#pragma once

#include <TextureBase.hpp>

namespace libgl
{

class FrameBuffer
{
public:
	static constexpr auto kEmptyHandle = (std::numeric_limits<GLuint>::max)();

	enum class BindingMode
	{
		FRAMEBUFFER = GL_FRAMEBUFFER, //both read & write
		DRAW_FRAMEBUFFER = GL_DRAW_FRAMEBUFFER,
		READ_FRAMEBUFFER = GL_READ_FRAMEBUFFER,
	};

	FrameBuffer(BindingMode mode);
	FrameBuffer(const FrameBuffer&) = delete;
	FrameBuffer(FrameBuffer&&) noexcept = delete;
	~FrameBuffer() noexcept;

	FrameBuffer& operator=(const FrameBuffer&) = delete;
	FrameBuffer& operator=(FrameBuffer&&) noexcept = delete;

	void bind(const TextureBase& texture);
	void unbind();
private:
	BindingMode m_bindngMode;
	GLuint m_framebuffer = kEmptyHandle;
};

}
