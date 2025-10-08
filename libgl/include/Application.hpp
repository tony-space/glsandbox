#pragma once

struct GLFWwindow;

namespace libgl
{

class Application
{
public:
	Application(const std::filesystem::path& projectDir);
	~Application();

	void run();
	void cursorPos(double x, double y);
private:
	std::filesystem::path m_projectDir;
	GLFWwindow* m_window;
};

}
