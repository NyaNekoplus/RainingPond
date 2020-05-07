#ifndef UTIL_H_
#define UTIL_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

namespace Util {
	struct TBvertex{
		glm::vec3 tangent;
		glm::vec3 bitangent;
	};
	static const float SCR_WIDTH = 800.0f;
	static const float SCR_HEIGHT = 600.0f;

	static const size_t PLANE_SIZE = 30;
	
	constexpr auto m = 0x100000000LL  ;
	constexpr auto c = 0xB16;
	constexpr auto a = 0x5DEECE66DLL;

	static thread_local unsigned long long seed = 1;

	double drand48(void);
	double MinusOneToOne(void);

	void CreateTBVertex(glm::vec3& tangent1, glm::vec3& bitangent1, glm::vec3& tangent2, glm::vec3& bitangent2);
	void CreatePlaneVertex(std::vector<glm::vec3>& vertex, std::vector<glm::vec2>& tex, std::vector<size_t>& indices, size_t rowLen, size_t colLen);
	void CreatePlaneVertex(std::vector<glm::vec3>& vertex, std::vector<glm::vec2>& tex, std::vector<TBvertex>& tbvertex, size_t rowLen, size_t colLen);
	float* GetPoint(float u, float v, float r);
	void CreatSphereVertex(std::vector<float>& vertex, size_t longtitude, size_t latitude, float r);
	int sign(double k);
	glm::mat4 GetReflectMatrix(glm::vec3 normal = glm::vec3(0, 1, 0), glm::vec3 p = glm::vec3(0, 0, 1));
	glm::mat4 CaculateObliqueViewFrustumMatrix(glm::mat4 projection, glm::mat4 reflectView, glm::vec4 plane = glm::vec4(0, 1, 0, -0));
}

#endif