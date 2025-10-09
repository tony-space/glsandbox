#pragma once

#include <TextureBase.hpp>

namespace libgl
{

//
// https://www.khronos.org/opengl/wiki/Texture_Storage#Mutable_storage
//
class MutableTexture : public TextureBase
{
public:

	MutableTexture(const MutableTexture&) = delete;
	MutableTexture(MutableTexture&&) noexcept = default;
	MutableTexture& operator=(const MutableTexture&) = delete;
	MutableTexture& operator=(MutableTexture&&) noexcept = default;
	
	void load(const TextureData& data);

	static MutableTexture make2D(GLsizei width, GLsizei height, TextureDeviceFormat format);
private:
	using TextureBase::TextureBase;
};

}