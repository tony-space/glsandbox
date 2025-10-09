#include <TextureBase.hpp>

namespace libgl
{

TextureBase::TextureBase(TextureTarget target, TextureDeviceFormat deviceFormat, GLsizei width, GLsizei height) :
	TextureBase(kEmptyHandle, target, deviceFormat, width, height, 1)
{
}

TextureBase::TextureBase(TextureTarget target, TextureDeviceFormat deviceFormat, GLsizei width, GLsizei height, GLsizei depth) :
	TextureBase(kEmptyHandle, target, deviceFormat, width, height, depth)
{
}

TextureBase::TextureBase(GLuint texture, TextureTarget target, TextureDeviceFormat deviceFormat, GLsizei width, GLsizei height, GLsizei depth) :
	m_target(target),
	m_deviceFormat(deviceFormat),
	m_width(width),
	m_height(height),
	m_depth(depth),
	m_texture(texture)
{
	if (texture == kEmptyHandle)
	{
		glGenTextures(1, &m_texture);
		checkGl();
	}
}

TextureBase::TextureBase(TextureBase&& o) noexcept
{
	std::swap(o.m_target, m_target);
	std::swap(o.m_deviceFormat, m_deviceFormat);
	std::swap(o.m_width, m_width);
	std::swap(o.m_height, m_height);
	std::swap(o.m_depth, m_depth);
	std::swap(o.m_texture, m_texture);
	std::swap(o.m_hasMipMaps, m_hasMipMaps);
}

TextureBase::~TextureBase() noexcept
{
	if (m_texture != kEmptyHandle)
	{
		glDeleteTextures(1, &m_texture);
		checkGl();
	}
}

TextureBase& TextureBase::operator=(TextureBase&& o) noexcept
{
	if (&o != this)
	{
		std::swap(o.m_target, m_target);
		std::swap(o.m_deviceFormat, m_deviceFormat);
		std::swap(o.m_width, m_width);
		std::swap(o.m_height, m_height);
		std::swap(o.m_depth, m_depth);
		std::swap(o.m_texture, m_texture);
		std::swap(o.m_hasMipMaps, m_hasMipMaps);
	}
	return *this;
}

void TextureBase::bind(GLuint slot)
{
	GLenum target = static_cast<GLenum>(m_target);
	glActiveTexture(GL_TEXTURE0 + slot);
	checkGl();

	glBindTexture(target, m_texture);
	checkGl();
}

void TextureBase::unbind(GLuint slot)
{
	GLenum target = static_cast<GLenum>(m_target);
	glActiveTexture(GL_TEXTURE0 + slot);
	checkGl();

	glBindTexture(target, 0);
	checkGl();
}

void TextureBase::generateMipmap()
{
	if (m_hasMipMaps) return;

	glGenerateMipmap(static_cast<GLenum>(m_target));
	checkGl();

	m_hasMipMaps = true;
}

}
