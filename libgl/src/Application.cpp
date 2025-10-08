#include <Application.hpp>

#include <ShaderProgram.hpp>
#include <BufferObject.hpp>
#include <Mesh.hpp>

namespace libgl
{

static Application* g_appInstance{ nullptr };

static GLFWwindow* createAppWindow()
{
	if (!glfwInit())
	{
		throw std::runtime_error("glfw init failed");
	}

	// OpenGL 4.3 includes OpenGL ES 3.0 as a subset
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	if (auto window = glfwCreateWindow(1366, 768, "", nullptr, nullptr))
	{
		glfwMakeContextCurrent(window);
		glfwSetCursorPosCallback(window, +[](GLFWwindow*, double xpos, double ypos)
		{
			g_appInstance->cursorPos(xpos, ypos);
		});

		if (glewInit() != GLEW_OK)
		{
			throw std::runtime_error("glewInit failed");
		}
		
		return window;
	}

	throw std::runtime_error("glfw failed to create a window");
}

Application::Application(const std::filesystem::path& projectDir) 
	: m_projectDir(projectDir)
	, m_window(createAppWindow())
{
	g_appInstance = this;
}

Application::~Application()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Application::run()
{
	auto vs = std::make_shared<VertexShader>(fetchString(m_projectDir / "assets/shaders/blit1.vs.glsl"));
	auto fs = std::make_shared<FragmentShader>(fetchString(m_projectDir / "assets/shaders/blit1.fs.glsl"));
	auto program = std::make_shared<ShaderProgram>(std::vector<std::shared_ptr<ShaderBase>>{ vs, fs });

	auto cube = Mesh::cube();

	BufferObject<glm::vec3> positions(BufferTarget::ARRAY_BUFFER);
	positions.setData(BufferUsage::STATIC_DRAW, cube.positions());
	BufferObject<glm::vec3> normals(BufferTarget::ARRAY_BUFFER);
	normals.setData(BufferUsage::STATIC_DRAW, cube.normals());
	BufferObject<glm::vec2> texCoords0(BufferTarget::ARRAY_BUFFER);
	texCoords0.setData(BufferUsage::STATIC_DRAW, cube.texCoords0());
	texCoords0.unbind();

	BufferObject<glm::u16vec3> indices(BufferTarget::ELEMENT_ARRAY_BUFFER);
	indices.setData(BufferUsage::STATIC_DRAW, cube.triangles());

	while (!glfwWindowShouldClose(m_window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(m_window);

		glfwPollEvents();
	}
}

void Application::cursorPos(double x, double y)
{
	(void)x;
	(void)y;
	//std::cout << x << ' ' << y << '\n';
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
