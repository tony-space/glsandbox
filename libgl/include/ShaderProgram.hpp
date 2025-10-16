#pragma once

#include <ShaderBase.hpp>

#include <glm.hpp>
#include <string_view>
#include <vector>

namespace std::filesystem
{
class path;
}

namespace libgl
{

class ShaderProgram
{
public:
	explicit ShaderProgram();
	explicit ShaderProgram(std::shared_ptr<VertexShader>, std::shared_ptr<FragmentShader>);
	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram(ShaderProgram&&) noexcept = delete;
	~ShaderProgram();

	ShaderProgram& operator=(const ShaderProgram&) = delete;
	ShaderProgram& operator=(ShaderProgram&&) noexcept = delete;

	const std::string& linkLog() const noexcept { return m_linkLog; }
	[[nodiscard]] GLint uniformLoc(const std::string_view& name) const noexcept;
	[[nodiscard]] GLint attribLoc(const std::string_view& name) const noexcept;
	void setUniform(const std::string_view& name, GLint scalar);
	void setUniform(const std::string_view& name, GLfloat scalar);
	void setUniform(const std::string_view& name, GLboolean scalar);
	void setUniform(const std::string_view& name, bool scalar);
	void setUniform(const std::string_view& name, GLfloat x, GLfloat y);
	void setUniform(const std::string_view& name, GLfloat x, GLfloat y, GLfloat z);
	void setUniform(const std::string_view& name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	void setUniform(const std::string_view& name, const glm::mat3& mat);
	void setUniform(const std::string_view& name, const glm::mat4& mat);
	void setUniform(const std::string_view& name, const glm::ivec2& vec);
	void setUniform(const std::string_view& name, const glm::vec2& vec);
	void setUniform(const std::string_view& name, const glm::vec3& vec);
	void setUniform(const std::string_view& name, const glm::vec4& vec);
	void setUniformVec2(const std::string_view& name, const GLfloat* vec2);
	void setUniformIVec2(const std::string_view& name, const GLint* ivec2);
	void setUniformVec3(const std::string_view& name, const GLfloat* vec3);
	void setUniformVec4(const std::string_view& name, const GLfloat* vec4);
	void setUniformMat3(const std::string_view& name, const GLfloat* mat3x3);
	void setUniformMat4(const std::string_view& name, const GLfloat* mat4x4);
	void setUniform1Array(const std::string_view& name, const GLfloat* array, GLsizei count);

	GLuint nativeHandle() const noexcept { return m_program; }

	void bind() noexcept;
	void unbind() noexcept;

	void validateProgram();
	std::vector<char> binary() const;

	static std::shared_ptr<ShaderProgram> make(const std::filesystem::path& fragmentPath, const std::filesystem::path& vertexPath);
private:
	constexpr static GLuint kInvalidId = (std::numeric_limits<GLuint>::max)();
	
	GLuint m_program{ kInvalidId };
	std::string m_linkLog;

	mutable std::vector<std::pair<std::string, GLint>> m_uniformLocations;
	mutable std::vector<std::pair<std::string, GLint>> m_attributeLocations;

	[[nodiscard]] bool attachAndCompile(std::shared_ptr<VertexShader>, std::shared_ptr<FragmentShader>);
	bool trySetBinary(GLenum format, const void* binary, size_t length);
};

}
