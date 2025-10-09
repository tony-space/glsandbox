#include <MutableTexture.hpp>

#include <cassert>

namespace libgl
{

void MutableTexture::load(const TextureData& data)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, data.rowAlignment);
	checkGl();

	switch (m_target)
	{
	case TextureTarget::TEXTURE_2D:
		glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLenum>(m_deviceFormat), m_width, m_height, 0, static_cast<GLenum>(data.format), static_cast<GLenum>(data.type), data.data);
		checkGl();
		break;
	default:
		assert(false);
	}
}

MutableTexture MutableTexture::make2D(GLsizei width, GLsizei height, TextureDeviceFormat format)
{
	return MutableTexture(TextureTarget::TEXTURE_2D, format, width, height);
}

}
