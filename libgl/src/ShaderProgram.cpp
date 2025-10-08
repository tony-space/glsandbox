#include <ShaderProgram.hpp>


namespace libgl
{

ShaderProgram::ShaderProgram(std::vector<std::shared_ptr<ShaderBase>> shaders)
{
	m_program = glCreateProgram();
	checkGl();

	if (!attachAndCompile(std::move(shaders)))
	{
		throw std::invalid_argument(linkLog());
	}
}

ShaderProgram::~ShaderProgram()
{
	if (m_program != kInvalidId)
	{
		glDeleteProgram(m_program);
		checkGl();
	}
}

GLint ShaderProgram::uniformLoc(const std::string_view& name) const noexcept
{
	if (auto it = std::find_if(m_uniformLocations.begin(), m_uniformLocations.end(),
		[&](const auto& pair)
	{
		return pair.first == name;
	}); it != m_uniformLocations.end())
	{
		return it->second;
	}
	GLint location = glGetUniformLocation(m_program, name.data());
	checkGl();

	assert(location != -1);

	m_uniformLocations.emplace_back(name, location);
	return location;
}

GLint ShaderProgram::attribLoc(const std::string_view& name) const noexcept
{
	if (auto it = std::find_if(m_attributeLocations.begin(), m_attributeLocations.end(),
		[&](const auto& pair)
	{
		return pair.first == name;
	}); it != m_attributeLocations.end())
	{
		return it->second;
	}
	GLint location = glGetAttribLocation(m_program, name.data());
	checkGl();

	assert(location != -1);

	m_attributeLocations.emplace_back(name, location);
	return location;
}

void ShaderProgram::setUniform(const std::string_view& name, GLint scalar)
{
	const auto loc = uniformLoc(name);
	glUniform1i(loc, scalar);
	checkGl();
}
void ShaderProgram::setUniform(const std::string_view& name, GLfloat scalar)
{
	const auto loc = uniformLoc(name);
	glUniform1f(loc, scalar);
	checkGl();
}
void ShaderProgram::setUniform(const std::string_view& name, GLboolean scalar)
{
	const auto loc = uniformLoc(name);
	glUniform1i(loc, static_cast<GLint>(scalar));
	checkGl();
}
void ShaderProgram::setUniform(const std::string_view& name, bool scalar)
{
	const auto loc = uniformLoc(name);
	glUniform1i(loc, static_cast<GLint>(scalar));
	checkGl();
}
void ShaderProgram::setUniform(const std::string_view& name, GLfloat x, GLfloat y)
{
	const auto loc = uniformLoc(name);
	glUniform2f(loc, x, y);
	checkGl();
}

void ShaderProgram::setUniform(const std::string_view& name, GLfloat x, GLfloat y, GLfloat z)
{
	const auto loc = uniformLoc(name);
	glUniform3f(loc, x, y, z);
	checkGl();
}

void ShaderProgram::setUniform(const std::string_view& name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	const auto loc = uniformLoc(name);
	glUniform4f(loc, x, y, z, w);
	checkGl();
}

void ShaderProgram::setUniform(const std::string_view& name, const glm::mat3& mat)
{
	setUniformMat3(name, glm::value_ptr(mat));
}

void ShaderProgram::setUniform(const std::string_view& name, const glm::mat4& mat)
{
	setUniformMat4(name, glm::value_ptr(mat));
}

void ShaderProgram::setUniform(const std::string_view& name, const glm::ivec2& vec)
{
	setUniformIVec2(name, glm::value_ptr(vec));
}

void ShaderProgram::setUniform(const std::string_view& name, const glm::vec2& vec)
{
	setUniformVec2(name, glm::value_ptr(vec));
}

void ShaderProgram::setUniform(const std::string_view& name, const glm::vec3& vec)
{
	setUniformVec3(name, glm::value_ptr(vec));
}

void ShaderProgram::setUniform(const std::string_view& name, const glm::vec4& vec)
{
	setUniformVec4(name, glm::value_ptr(vec));
}

void ShaderProgram::setUniformVec2(const std::string_view& name, const GLfloat* vec2)
{
	const auto loc = uniformLoc(name);
	glUniform2fv(loc, 1, vec2);
	checkGl();
}

void ShaderProgram::setUniformIVec2(const std::string_view& name, const GLint* ivec2)
{
	const auto loc = uniformLoc(name);
	glUniform2iv(loc, 1, ivec2);
	checkGl();
}

void ShaderProgram::setUniformVec3(const std::string_view& name, const GLfloat* vec3)
{
	const auto loc = uniformLoc(name);
	glUniform3fv(loc, 1, vec3);
	checkGl();
}

void ShaderProgram::setUniformVec4(const std::string_view& name, const GLfloat* vec4)
{
	const auto loc = uniformLoc(name);
	glUniform4fv(loc, 1, vec4);
	checkGl();
}

void ShaderProgram::setUniformMat3(const std::string_view& name, const GLfloat* mat3x3)
{
	const auto loc = uniformLoc(name);
	glUniformMatrix3fv(loc, 1, GL_FALSE, mat3x3);
	checkGl();
}

void ShaderProgram::setUniformMat4(const std::string_view& name, const GLfloat* mat4x4)
{
	const auto loc = uniformLoc(name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, mat4x4);
	checkGl();
}
void ShaderProgram::setUniform1Array(const std::string_view& name, const GLfloat* array, GLsizei count)
{
	const auto loc = uniformLoc(name);
	glUniform1fv(loc, count, array);
	checkGl();
}

void ShaderProgram::bind() noexcept
{
	glUseProgram(m_program);
	checkGl();
}

void ShaderProgram::unbind() noexcept
{
	glUseProgram(0);
	checkGl();
}

bool ShaderProgram::attachAndCompile(std::vector<std::shared_ptr<ShaderBase>> shaders)
{
	std::vector<GLchar> rawLog;

	for (const auto& shader : shaders)
	{
		glAttachShader(m_program, shader->nativeHandle());
		checkGl();
	}

	glLinkProgram(m_program);
	checkGl();

	GLint linkInfoLog;
	glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &linkInfoLog);
	checkGl();

	if (linkInfoLog)
	{
		rawLog.resize(linkInfoLog);
		glGetProgramInfoLog(m_program, linkInfoLog, nullptr, rawLog.data());
		m_linkLog = std::string(rawLog.cbegin(), rawLog.cend());
	}

	GLint linkStatus;
	glGetProgramiv(m_program, GL_LINK_STATUS, &linkStatus);
	checkGl();
	if (!linkStatus)
		return false;

	m_shaders = std::move(shaders);

	return true;
}

}
