#include <Application.hpp>

#include <iostream>

static std::filesystem::path projectDir()
{
	for (auto workingDirectory = std::filesystem::current_path(); !workingDirectory.empty(); workingDirectory = workingDirectory.parent_path())
	{
		auto candidate = workingDirectory / "assets";
		if (std::filesystem::is_directory(candidate))
		{
			return workingDirectory;
		}
	}

	throw std::runtime_error("cannot find project root location");
}

int main()
{
	try
	{
		libgl::Application app(projectDir());

		app.run();
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what();
	}
}
