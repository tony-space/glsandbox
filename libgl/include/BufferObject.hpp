#pragma once

namespace libgl
{

// https://registry.khronos.org/OpenGL-Refpages/es3.0/html/glBufferData.xhtml
enum class BufferTarget
{
	ARRAY_BUFFER = GL_ARRAY_BUFFER,
	ELEMENT_ARRAY_BUFFER = GL_ELEMENT_ARRAY_BUFFER,
	PIXEL_PACK_BUFFER = GL_PIXEL_PACK_BUFFER,
	PIXEL_UNPACK_BUFFER = GL_PIXEL_UNPACK_BUFFER
};

// https://registry.khronos.org/OpenGL-Refpages/es3.0/html/glBufferData.xhtml
enum class BufferUsage
{
	STREAM_DRAW = GL_STREAM_DRAW,
	STREAM_READ = GL_STREAM_READ,
	STREAM_COPY = GL_STREAM_COPY,
	
	STATIC_DRAW = GL_STATIC_DRAW,
	STATIC_READ = GL_STATIC_READ,
	STATIC_COPY = GL_STATIC_COPY,
	
	DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
	DYNAMIC_READ = GL_DYNAMIC_READ,
	DYNAMIC_COPY = GL_DYNAMIC_COPY
};

// https://registry.khronos.org/OpenGL-Refpages/es3.0/html/glMapBufferRange.xhtml
enum class BufferAccessFlags
{
	MAP_READ_BIT = GL_MAP_READ_BIT,
	MAP_WRITE_BIT = GL_MAP_WRITE_BIT,

	MAP_INVALIDATE_RANGE_BIT = GL_MAP_INVALIDATE_RANGE_BIT,
	MAP_INVALIDATE_BUFFER_BIT = GL_MAP_INVALIDATE_BUFFER_BIT,
	MAP_FLUSH_EXPLICIT_BIT = GL_MAP_FLUSH_EXPLICIT_BIT,
	MAP_UNSYNCHRONIZED_BIT = GL_MAP_UNSYNCHRONIZED_BIT,
};

inline BufferAccessFlags operator| (BufferAccessFlags lhs, BufferAccessFlags rhs)
{
	return static_cast<BufferAccessFlags>(static_cast<int>(lhs) | static_cast<int>(rhs));
}

class BufferObjectBase
{
public:
	static constexpr auto kEmptyHandle = (std::numeric_limits<GLuint>::max)();

	BufferObjectBase(BufferTarget target);
	BufferObjectBase(const BufferObjectBase&) = delete;
	BufferObjectBase(BufferObjectBase&&) noexcept;
	~BufferObjectBase() noexcept;

	BufferObjectBase& operator=(const BufferObjectBase&) = delete;
	BufferObjectBase& operator=(BufferObjectBase&&) noexcept;

	void validateBinding(BufferObjectBase* self) const;
	void bind();
	void unbind();
	
	static bool supportsMapBufferRange() noexcept;
protected:
	GLuint m_buffer = kEmptyHandle;
	BufferTarget m_target;
};



template <typename...>
class BufferObject;

template <glm::length_t N, typename T>
class BufferObject <glm::vec<N, T>> : public BufferObjectBase
{
public:
	using underlying_type = glm::vec<N, T>;
	using BufferObjectBase::BufferObjectBase;

	void reserve(BufferUsage usage, std::size_t elementsCount)
	{
		bind();
		glBufferData(static_cast<GLenum>(m_target), elementsCount * sizeof(underlying_type), nullptr, static_cast<GLenum>(usage));
		checkGl();
	}

	void setData(BufferUsage usage, const underlying_type* begin, const underlying_type *end)
	{
		const auto elementsCount = end - begin;

		bind();
		glBufferData(static_cast<GLenum>(m_target), elementsCount * sizeof(underlying_type), begin, static_cast<GLenum>(usage));
		checkGl();
	}

	void setData(BufferUsage usage, const std::vector<underlying_type>& vec)
	{
		setData(usage, vec.data(), vec.data() + vec.size());
	}

};

}
