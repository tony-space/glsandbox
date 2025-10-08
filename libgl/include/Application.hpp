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
	
	static std::vector<std::uint8_t> fetchContent(const std::filesystem::path& path);
	static std::string fetchString(const std::filesystem::path& path);
private:
	std::filesystem::path m_projectDir;
	GLFWwindow* m_window;
};

}
