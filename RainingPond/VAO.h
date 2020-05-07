#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "Shader.h"
#include "util.h"

namespace OpenGL{
	class Shader;

	class VAO {
	public:
		VAO(const float* data, size_t dataSize, const std::vector<size_t>& attrCount);//占点的数量
		VAO(const float* data, size_t dataSize, const std::vector<size_t>& attrCount, const size_t* indices, const size_t indiceSize);
		VAO(const std::vector<glm::vec3>& vertexData,
			const std::vector<glm::vec2>& texData);
		VAO(const std::vector<glm::vec3>& vertexData,
			const std::vector<glm::vec2>& texData,
			const std::vector<size_t>& indices);
		VAO(const std::vector<glm::vec3>& vertexData,
			const std::vector<glm::vec2>& texData,
			const std::vector<Util::TBvertex>& TBvertex);
		size_t GetID()const;
		bool IsValid()const;
		bool Bind()const;
		bool Draw()const;
	private:
		bool BindEBO(const size_t* indices, const size_t dataSize);
		bool BindEBO(const std::vector<size_t>& indices);
		size_t ID;
		size_t PointNum;
		bool isValid;
		bool hasIndex;
	};
}
