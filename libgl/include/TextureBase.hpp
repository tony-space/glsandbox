#pragma once

#include <opengl.hpp>

namespace libgl
{

enum class TextureTarget
{
	TEXTURE_2D = GL_TEXTURE_2D,
};

enum class TextureDeviceFormat
{
	SRGB8 = GL_SRGB8,
	SRGB8_ALPHA8 = GL_SRGB8_ALPHA8,

	R8 = GL_R8,
	RG8 = GL_RG8,
	RGB8 = GL_RGB8,
	RGBA8 = GL_RGBA8,

	R16F = GL_R16F,
	RG16F = GL_RG16F,
	RGB16F = GL_RGB16F,
	RGBA16F = GL_RGBA16F,
	
	R32F = GL_R32F,
	RG32F = GL_RG32F,
	RGB32F = GL_RGB32F,
	RGBA32F = GL_RGBA32F,
};

enum class TextureHostFormat
{
	RED = GL_RED,
	RG = GL_RG,
	RGB = GL_RGB,
	RGBA = GL_RGBA
};

enum class TextureHostType
{
	UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
	FLOAT = GL_FLOAT,
};

struct TextureData
{
	TextureHostFormat format;
	TextureHostType type;
	GLint rowAlignment{ 4 };
	const void* data;
};

class TextureBase
{
public:
	static constexpr inline auto kEmptyHandle = (std::numeric_limits<GLuint>::max)();

	TextureBase(const TextureBase&) = delete;
	TextureBase(TextureBase&&) noexcept;
	~TextureBase() noexcept;

	TextureBase& operator=(const TextureBase&) = delete;
	TextureBase& operator=(TextureBase&&) noexcept;

	void generateMipmap();

	void bind(GLuint slot);
	void unbind(GLuint slot);
	TextureTarget target() const noexcept { return m_target; }
	TextureDeviceFormat deviceFormat() const noexcept { return m_deviceFormat; }
	GLsizei width() const noexcept { return m_width; }
	GLsizei height() const noexcept { return m_height; }
	GLsizei depth() const noexcept { return m_depth; }
	GLuint nativeHandle() const noexcept { return m_texture; }
	bool hasMipmaps() const noexcept { return m_hasMipMaps; }

	void setHasMipmaps(bool value) noexcept { m_hasMipMaps = value; }

protected:
	TextureBase(TextureTarget, TextureDeviceFormat, GLsizei width, GLsizei height);
	TextureBase(TextureTarget, TextureDeviceFormat, GLsizei width, GLsizei height, GLsizei depth);
	TextureBase(GLuint texture, TextureTarget, TextureDeviceFormat, GLsizei width, GLsizei height, GLsizei depth);

	TextureTarget m_target;
	TextureDeviceFormat m_deviceFormat;
	GLsizei m_width;
	GLsizei m_height;
	GLsizei m_depth{ 1 };
	GLuint m_texture = kEmptyHandle;
	bool m_hasMipMaps{ false };
};

}
