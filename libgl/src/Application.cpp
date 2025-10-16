#include <Application.hpp>

#include <stb/stb_image.h>
#include <iostream>
#include <fstream>

namespace libgl
{

#ifdef MACOSX
static constexpr bool kMacOS = true;
#else
static constexpr bool kMacOS = false;
#endif

static constexpr int kWidth = 1366;
static constexpr int kHeight = 768;

static Application* g_appInstance{ nullptr };

#ifndef NDEBUG
static const char* getDebugSource(GLenum source)
{
	switch (source)
	{
		case GL_DEBUG_SOURCE_API: return "GL_DEBUG_SOURCE_API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "GL_DEBUG_SOURCE_WINDOW_SYSTEM";
		case GL_DEBUG_SOURCE_SHADER_COMPILER: return "GL_DEBUG_SOURCE_SHADER_COMPILER";
		case GL_DEBUG_SOURCE_THIRD_PARTY: return "GL_DEBUG_SOURCE_THIRD_PARTY";
		case GL_DEBUG_SOURCE_APPLICATION: return "GL_DEBUG_SOURCE_APPLICATION";
		case GL_DEBUG_SOURCE_OTHER: return "GL_DEBUG_SOURCE_OTHER";
	}
	std::terminate();
}
static const char* getDebugType(GLenum type)
{
	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR: return "GL_DEBUG_TYPE_ERROR";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR";
		case GL_DEBUG_TYPE_PORTABILITY: return "GL_DEBUG_TYPE_PORTABILITY";
		case GL_DEBUG_TYPE_PERFORMANCE: return "GL_DEBUG_TYPE_PERFORMANCE";
		case GL_DEBUG_TYPE_OTHER: return "GL_DEBUG_TYPE_OTHER";
		case GL_DEBUG_TYPE_MARKER: return "GL_DEBUG_TYPE_MARKER";
		case GL_DEBUG_TYPE_PUSH_GROUP: return "GL_DEBUG_TYPE_PUSH_GROUP";
		case GL_DEBUG_TYPE_POP_GROUP: return "GL_DEBUG_TYPE_POP_GROUP";
	}
	std::terminate();
}
static const char* getDebugSeverity(GLenum severity)
{
	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH: return "GL_DEBUG_SEVERITY_HIGH";
		case GL_DEBUG_SEVERITY_MEDIUM: return "GL_DEBUG_SEVERITY_MEDIUM";
		case GL_DEBUG_SEVERITY_LOW: return "GL_DEBUG_SEVERITY_LOW";
		case GL_DEBUG_SEVERITY_NOTIFICATION: return "GL_DEBUG_SEVERITY_NOTIFICATION";
	}

	std::terminate();
}
#endif // !NDEBUG

static auto createAppWindow()
{
	if (!glfwInit())
	{
		throw std::runtime_error("glfw init failed");
	}

	if constexpr (kMacOS)
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	}
	else
	{
		// OpenGL 4.3 includes OpenGL ES 3.0 as a subset
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	}
	
	
	glfwWindowHint(GLFW_SCALE_FRAMEBUFFER, GLFW_FALSE);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 8);


	if (auto window = glfwCreateWindow(kWidth, kHeight, "glsandbox", nullptr, nullptr))
	{
		glfwMakeContextCurrent(window);
		glfwSwapInterval(1);
		glfwSetWindowSizeCallback(window, +[](GLFWwindow*, int width, int height)
		{
			g_appInstance->resize(width, height);
		});

		if (glewInit() != GLEW_OK)
		{
			throw std::runtime_error("glewInit failed");
		}

		std::cout << "" << "OpenGL Vendor: " << glGetString(GL_VENDOR) << '\n';
		std::cout << "" << "OpenGL Renderer: " << glGetString(GL_RENDERER) << '\n';
		std::cout << "" << "OpenGL Version: " << glGetString(GL_VERSION) << '\n';
		std::cout << "" << "OpenGL Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';

#ifndef NDEBUG
		if (glDebugMessageControl)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			checkGl()
			glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
			glDebugMessageControl(GL_DEBUG_SOURCE_WINDOW_SYSTEM, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
			glDebugMessageControl(GL_DEBUG_SOURCE_SHADER_COMPILER, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
			glDebugMessageControl(GL_DEBUG_SOURCE_THIRD_PARTY, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
			glDebugMessageControl(GL_DEBUG_SOURCE_APPLICATION, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
			glDebugMessageControl(GL_DEBUG_SOURCE_OTHER, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

			glDebugMessageCallback(+[](GLenum source, GLenum type, GLuint /*id*/,
				GLenum severity, GLsizei /*length*/,
				const GLchar* message, const void* /*userParam*/)
			{
				std::cout
					<< '['
					<< getDebugSource(source) << "] ["
					<< getDebugType(type) << "] ["
					<< getDebugSeverity(severity) << "] : "
					<< message << '\n';
			}, nullptr);
		}
#endif // !NDEBUG
		
		
		return std::shared_ptr<GLFWwindow>(window, +[](GLFWwindow* window)
		{
			glfwDestroyWindow(window);
		});
	}

	throw std::runtime_error("glfw failed to create a window");
}

static MutableTexture loadImage(const std::filesystem::path& path)
{
	const auto rawData = Application::fetchContent(path);

	int width;
	int height;
	int channels;
	auto data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(rawData.data()), static_cast<int>(rawData.size()), &width, &height, &channels, 0);

	if (data == nullptr)
	{
		throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory), path.string());
	}

	TextureData hostData;
	hostData.data = data;
	hostData.rowAlignment = 1;
	hostData.type = TextureHostType::UNSIGNED_BYTE;
	switch (channels)
	{
	case 1: hostData.format = TextureHostFormat::RED; break;
	case 2: hostData.format = TextureHostFormat::RG; break;
	case 3: hostData.format = TextureHostFormat::RGB; break;
	case 4: hostData.format = TextureHostFormat::RGBA; break;
	default: assert(false);
	}

	try
	{
		MutableTexture texture = MutableTexture::make2D(width, height, TextureDeviceFormat::SRGB8_ALPHA8);
		texture.bind(0);
		texture.load(hostData);
		stbi_image_free(data);
		return texture;
	}
	catch (...)
	{
		stbi_image_free(data);
		throw;
	}
}

Application::Application(const std::filesystem::path& projectDir) 
	: m_projectDir(projectDir)
	, m_window(createAppWindow())
{
	g_appInstance = this;

	m_program = ShaderProgram::make(m_projectDir / "assets/shaders/blit1.vs.glsl", m_projectDir / "assets/shaders/blit1.fs.glsl");

	glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
	glDisable(GL_CULL_FACE);
	glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

	glEnable(GL_SAMPLE_SHADING);
	glMinSampleShading(8);
	checkGl();


	m_vao = std::make_shared<VertexArrayObject>();
	m_vao->bind();

	auto cube = Mesh::cube();

	m_meshData = std::make_shared<BufferData>();
	
	m_meshData->positions.setData(BufferUsage::STATIC_DRAW, cube.positions());
	m_vao->setVertexAttribute(m_program->attribLoc("A_POSITION_0"), VertexAttribute::make<glm::vec3>());

	m_meshData->normals.setData(BufferUsage::STATIC_DRAW, cube.normals());
	m_vao->setVertexAttribute(m_program->attribLoc("A_NORMAL_0"), VertexAttribute::make<glm::vec3>());

	m_meshData->texCoords0.setData(BufferUsage::STATIC_DRAW, cube.texCoords0());
	m_vao->setVertexAttribute(m_program->attribLoc("A_TEX_COORD_0"), VertexAttribute::make<glm::vec2>());

	m_meshData->indices.setData(BufferUsage::STATIC_DRAW, cube.triangles());
	m_meshData->indicesCount = cube.triangles().size();

	m_texture = std::make_shared<MutableTexture>(loadImage(m_projectDir / "assets/png/grid.png"));
	m_texture->generateMipmap();
	glTexParameteri((GLenum)m_texture->target(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri((GLenum)m_texture->target(), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri((GLenum)m_texture->target(), GL_TEXTURE_MAX_ANISOTROPY, 16);

	std::pair<int, int> winDim;
	glfwGetWindowSize(m_window.get(), &winDim.first, &winDim.second);
	resize(winDim.first, winDim.second);
}

void Application::run()
{
	m_program->bind();
	m_vao->bind();
	m_meshData->indices.bind();
	
	m_texture->bind(0);
	m_program->setUniform("U_SAMPLER_0", 0);
	m_program->setUniform("U_LIGHT_DIR_0", glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));

	m_program->validateProgram();

	const auto startTime = std::chrono::high_resolution_clock::now();

	while (!glfwWindowShouldClose(m_window.get()))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		checkGl();

		const auto timeSec 
			= std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - startTime).count() / 1e6f;

		auto viewMat = glm::identity<glm::mat4>();
		viewMat = glm::translate(viewMat, glm::vec3(0.0f, 0.0f, -2.0f));
		viewMat = glm::rotate(viewMat, timeSec * 0.25f, glm::vec3(1.0f, 0.0f, 0.0f));
		viewMat = glm::rotate(viewMat, timeSec * 0.25f, glm::vec3(0.0f, 1.0f, 0.0f));
		viewMat = glm::rotate(viewMat, timeSec * 0.25f, glm::vec3(0.0f, 0.0f, 1.0f));

		m_program->setUniform("U_PROJECTION_TRANSFORM", m_projMatrix);
		m_program->setUniform("U_VIEW_TRANSFORM", viewMat);

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_meshData->indicesCount * 3), GL_UNSIGNED_SHORT, 0);
		checkGl();

		glfwSwapBuffers(m_window.get());
		glfwPollEvents();
	}
}

void Application::resize(int x, int y)
{
	glViewport(0, 0, x, y);
	checkGl();

	m_projMatrix = glm::perspective(glm::radians(60.0f), float(x) / y, 0.01f, 100.0f);
}

std::vector<std::uint8_t> Application::fetchContent(const std::filesystem::path& path)
{
	std::ifstream stream(path, std::ios_base::binary);
	assert(!stream.fail());
	if (stream.fail())
	{
		throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory));
	}

	return { std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>() };
}

}
