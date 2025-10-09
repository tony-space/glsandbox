#pragma once

namespace libgl
{

enum class AttributeType
{
	BYTE = GL_BYTE,
	UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
	SHORT = GL_SHORT,
	UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
	INT = GL_INT,
	UNSIGNED_INT = GL_UNSIGNED_INT,
	FLOAT = GL_FLOAT,
};

template <typename T>
struct attribute_type;

template<typename T>
constexpr inline auto attribute_type_v = attribute_type<T>::value;

template<> struct attribute_type<std::int8_t> { static constexpr AttributeType value = AttributeType::BYTE; };
template<> struct attribute_type<std::uint8_t> { static constexpr AttributeType value = AttributeType::UNSIGNED_BYTE; };
template<> struct attribute_type<std::int16_t> { static constexpr AttributeType value = AttributeType::SHORT; };
template<> struct attribute_type<std::uint16_t> { static constexpr AttributeType value = AttributeType::UNSIGNED_SHORT; };
template<> struct attribute_type<std::int32_t> { static constexpr AttributeType value = AttributeType::INT; };
template<> struct attribute_type<std::uint32_t> { static constexpr AttributeType value = AttributeType::UNSIGNED_INT; };
template<> struct attribute_type<float> { static constexpr AttributeType value = AttributeType::FLOAT; };

struct VertexAttribute
{
	GLint components{ 4 };
	AttributeType type = AttributeType::FLOAT;
	GLsizei bytesStride{ 4 * sizeof(float) };
	GLboolean normalized{ GL_FALSE };

	GLuint divisor{ 0u }; // 0 means 'per vertex', 1 means 'per instance', 2 means 'per two instances' and so on
	std::size_t byteOffset{ 0 };

	template <glm::length_t N, typename T>
	void setType()
	{
		components = N;
		type = attribute_type_v<T>;
		bytesStride = N * sizeof(T);
	}

	template <typename GlmVectorType>
	void setType()
	{
		setType<GlmVectorType::length(), typename GlmVectorType::value_type>();
	}

	template <typename GlmVectorType>
	static VertexAttribute make()
	{
		VertexAttribute result;
		result.setType<GlmVectorType>();

		return result;
	}
};

class VertexArrayObject
{
public:
	static constexpr auto kEmptyHandle = std::numeric_limits<GLuint>::max();

	VertexArrayObject();
	VertexArrayObject(const VertexArrayObject&) = delete;
	VertexArrayObject(VertexArrayObject&&) noexcept;
	~VertexArrayObject();

	VertexArrayObject& operator= (const VertexArrayObject&) = delete;
	VertexArrayObject& operator= (VertexArrayObject&&) noexcept;

	void bind();
	void unbind();


	void setVertexAttribute(GLint shaderAttributeLocation, const VertexAttribute&);
	void disableAttribute(GLint shaderAttributeLocation);
private:
	GLuint m_vao = kEmptyHandle;
};

}
