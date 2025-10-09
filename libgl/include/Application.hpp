#pragma once

#include <BufferObject.hpp>
#include <Mesh.hpp>
#include <MutableTexture.hpp>
#include <ShaderProgram.hpp>
#include <VertexArrayObject.hpp>

#include <filesystem>

struct GLFWwindow;

namespace libgl
{

class Application
{
public:
	Application(const std::filesystem::path& projectDir);

	void run();
	void resize(int x, int y);
	//void cursorPos(double x, double y);

	static std::vector<std::uint8_t> fetchContent(const std::filesystem::path& path);
	static std::string fetchString(const std::filesystem::path& path);
private:
	std::filesystem::path m_projectDir;
	GLFWwindow* m_window;

	struct BufferData
	{
		BufferObject<glm::vec3> positions{ BufferTarget::ARRAY_BUFFER };
		BufferObject<glm::vec3> normals{ BufferTarget::ARRAY_BUFFER };
		BufferObject<glm::vec2> texCoords0{ BufferTarget::ARRAY_BUFFER };
		BufferObject<glm::u16vec3> indices{ BufferTarget::ELEMENT_ARRAY_BUFFER };
		size_t indicesCount;
	};

	std::shared_ptr<ShaderProgram> m_program;
	std::shared_ptr<VertexArrayObject> m_vao;
	std::shared_ptr<BufferData> m_meshData;
	std::shared_ptr<MutableTexture> m_texture;

	glm::mat4 m_projMatrix;
};

}
