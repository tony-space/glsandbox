#include <Application.hpp>

namespace libgl
{

static Application* g_appInstance{ nullptr };

static GLFWwindow* createAppWindow()
{
	glewInit();

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

//std::vector<std::uint8_t> fetchContent(const std::filesystem::path& path)
//{
//	core::log::info("runtime") << "reading binary content from: " << path.string();

//	std::ifstream stream(path, std::ios_base::binary);
//	assert(!stream.fail());
//	if (stream.fail())
//	{
//		throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory));
//	}

//	return { std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>() };
//}

}
