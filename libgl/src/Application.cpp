#include <Application.hpp>

#include <stb/stb_image.h>
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

static auto createAppWindow()
{
	if (!glfwInit())
	{
		throw std::runtime_error("glfw init failed");
	}

	if constexpr (kMacOS)
	{
		// MacOS supports only GL 4.1 as core profile
		// 2.1 is compatible with ES 3.0 via Compatibility Profile and some extensions
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
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
		//glfwSetCursorPosCallback(window, +[](GLFWwindow*, double xpos, double ypos)
		//{
		//	g_appInstance->cursorPos(xpos, ypos);
		//});

		if (glewInit() != GLEW_OK)
		{
			throw std::runtime_error("glewInit failed");
		}

		
		return std::shared_ptr<GLFWwindow>(window, +[](GLFWwindow* window)
		{
			glfwDestroyWindow(window);
			auto err = glfwGetError(nullptr);
			assert(err == GLFW_NO_ERROR);
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

	auto vs = std::make_shared<VertexShader>(fetchString(m_projectDir / "assets/shaders/blit1.vs.glsl"));
	auto fs = std::make_shared<FragmentShader>(fetchString(m_projectDir / "assets/shaders/blit1.fs.glsl"));
	m_program = std::make_shared<ShaderProgram>(std::vector<std::shared_ptr<ShaderBase>>{ std::move(vs), std::move(fs) });

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

	glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	//glEnable(GL_CULL_FACE);
	checkGl();

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

//void Application::cursorPos(double x, double y)
//{
//	(void)x;
//	(void)y;
//	//std::cout << x << ' ' << y << '\n';
//}

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

std::string Application::fetchString(const std::filesystem::path& path)
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
