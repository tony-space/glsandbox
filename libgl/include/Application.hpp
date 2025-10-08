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
private:
	std::filesystem::path m_projectDir;
	GLFWwindow* m_window;
};

}
