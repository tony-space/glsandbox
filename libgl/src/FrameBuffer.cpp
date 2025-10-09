#include <ShaderBase.hpp>
#include <FrameBuffer.hpp>

#include <cassert>

namespace libgl
{

FrameBuffer::FrameBuffer(BindingMode mode) :
	m_bindngMode(mode)
{
	glGenFramebuffers(1, &m_framebuffer);
	checkGl();
}

FrameBuffer::~FrameBuffer() noexcept
{
	if (m_framebuffer != kEmptyHandle)
	{
		glDeleteFramebuffers(1, &m_framebuffer);
		checkGl();
	}
}

void FrameBuffer::bind(const TextureBase& texture)
{
	glBindFramebuffer(static_cast<GLenum>(m_bindngMode), m_framebuffer);
	checkGl();

	glViewport(0, 0, texture.width(), texture.height());
	checkGl();

	glFramebufferTexture2D(
		static_cast<GLenum>(m_bindngMode),
		GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D,
		texture.nativeHandle(),
		0);
	checkGl();

	assert(glCheckFramebufferStatus(static_cast<GLenum>(m_bindngMode)) == GL_FRAMEBUFFER_COMPLETE);
	checkGl();

	if (m_bindngMode == BindingMode::DRAW_FRAMEBUFFER)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		checkGl();
	}
}

void FrameBuffer::unbind()
{
	glFramebufferTexture2D(
	static_cast<GLenum>(m_bindngMode),
	GL_COLOR_ATTACHMENT0,
	GL_TEXTURE_2D,
	0,
	0);
	checkGl();

	glBindFramebuffer(static_cast<GLenum>(m_bindngMode), 0);
	checkGl();
}

}
