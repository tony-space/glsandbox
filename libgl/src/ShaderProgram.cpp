#include <ShaderProgram.hpp>

#include <fstream>
#include <filesystem>

namespace libgl
{

static std::string fetchString(const std::filesystem::path& path)
{
	std::ifstream stream(path);
	assert(!stream.fail());
	if (stream.fail())
	{
		throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory));
	}

	return { std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>() };
}

static std::vector<char> fetchBinary(const std::filesystem::path& path)
{
	std::ifstream stream(path, std::ios_base::binary);
	assert(!stream.fail());
	if (stream.fail())
	{
		throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory));
	}

	return { std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>() };
}

template<typename Iterator>
static void writeBinary(std::filesystem::path path, Iterator begin, Iterator end)
{
	std::filesystem::create_directory(path.parent_path());
	std::ofstream writeStream(path, std::ios_base::binary);
	assert(!writeStream.fail());
	if (writeStream.fail())
	{
		throw std::system_error(std::make_error_code(std::errc::permission_denied));
	}
	
	std::copy(begin, end, std::ostreambuf_iterator<char>(writeStream));
}

ShaderProgram::ShaderProgram()
{
	m_program = glCreateProgram();
	checkGl();
}

ShaderProgram::ShaderProgram(std::shared_ptr<VertexShader> vs, std::shared_ptr<FragmentShader> fs) : ShaderProgram()
{
	if (!attachAndCompile(std::move(vs), std::move(fs)))
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

bool ShaderProgram::attachAndCompile(std::shared_ptr<VertexShader> vs, std::shared_ptr<FragmentShader> fs)
{
	std::vector<GLchar> rawLog;

	glAttachShader(m_program, vs->nativeHandle());
	checkGl();

	glAttachShader(m_program, fs->nativeHandle());
	checkGl();

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

	return linkStatus;
}

void ShaderProgram::validateProgram()
{
	glValidateProgram(m_program);
	checkGl();

	GLint validateStatus;
	glGetProgramiv(m_program, GL_VALIDATE_STATUS, &validateStatus);
	checkGl();

	if (validateStatus)
		return;

	GLint validateInfoLogLen;
	glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &validateInfoLogLen);
	checkGl();

	if (!validateInfoLogLen)
		return;

	std::vector<GLchar> rawLog(validateInfoLogLen);
	glGetProgramInfoLog(m_program, validateInfoLogLen, nullptr, rawLog.data());

	auto message = std::string("Validation failed: ") + std::string(rawLog.cbegin(), rawLog.cend());
	throw std::runtime_error(message);
}

std::vector<char> ShaderProgram::binary() const
{
	GLint length;
	glGetProgramiv(m_program, GL_PROGRAM_BINARY_LENGTH, &length);
	checkGl();

	std::vector<char> buffer(sizeof(GLenum) + static_cast<size_t>(length));

	GLsizei writtenLength;
	glGetProgramBinary(
		m_program,
		static_cast<GLsizei>(length),
		&writtenLength,
		reinterpret_cast<GLenum*>(buffer.data()),
		buffer.data() + sizeof(GLenum));
	checkGl();

	return buffer;
}

bool ShaderProgram::trySetBinary(GLenum format, const void* binary, size_t length)
{
	glProgramBinary(m_program, format, binary, static_cast<GLsizei>(length));

	auto status = glGetError();
	if (status == GL_NO_ERROR)
	{
		return true;
	}

	if (status == GL_INVALID_ENUM)
	{
		return false;
	}

	detail::checkGLStatus(status, __FUNCTION__, __FILE__, __LINE__);
	std::terminate();
}

std::shared_ptr<ShaderProgram> ShaderProgram::make(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath)
{
	const auto shaderCacheDir = std::filesystem::current_path() / "shader_cache";

	auto vertexSource = fetchString(vertexPath);
	auto fragmentSource = fetchString(fragmentPath);

	auto programHash = std::hash<std::string>{}(vertexSource) ^ std::hash<std::string>{}(fragmentSource);
	auto fileName = shaderCacheDir / std::to_string(programHash);

	auto program = std::make_shared<ShaderProgram>();

	if (std::filesystem::exists(fileName))
	{
		auto binary = fetchBinary(fileName);
		if (program->trySetBinary(*reinterpret_cast<GLenum*>(binary.data()), binary.data() + sizeof(GLenum), binary.size() - sizeof(GLenum)))
		{
			return program;
		}
	}

	auto vs = std::make_shared<VertexShader>(vertexSource);
	auto fs = std::make_shared<FragmentShader>(fragmentSource);
	if (!program->attachAndCompile(std::move(vs), std::move(fs)))
	{
		throw std::invalid_argument(program->linkLog());
	}

	auto binary = program->binary();
	writeBinary(fileName, binary.cbegin(), binary.cend());

	return program;
}

}
