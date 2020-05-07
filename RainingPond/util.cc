#include "util.h"

double Util::drand48(void) {
	seed = (a * seed + c) & 0xFFFFFFFFFFFFLL;
	unsigned int x = seed >> 16;
	return  ((double)x / (double)m);
}
double Util::MinusOneToOne(void) {
	return (drand48() - 0.5) * 2.0;
}

void Util::CreateTBVertex(glm::vec3& tangent1, glm::vec3& bitangent1, glm::vec3& tangent2, glm::vec3& bitangent2) {
#if 0
	glm::vec3 pos1, pos2, pos3, pos4;
	pos1 = glm::vec3(-0.5, 0.5, 0.0);
	pos2 = glm::vec3(-0.5, -0.5, 0.0);
	pos3 = glm::vec3(0.5, -0.5, 0.0);
	pos4 = glm::vec3(0.5, 0.5, 0.0);

	// texture coordinates
	glm::vec2 uv1(0.0, 1.0);
	glm::vec2 uv2(0.0, 0.0);
	glm::vec2 uv3(1.0, 0.0);
	glm::vec2 uv4(1.0, 1.0);
	// normal vector
	glm::vec3 nm(0.0, 0.0, 1.0);
#else
	glm::vec3 pos1, pos2, pos3, pos4;
	pos1 = glm::vec3(-0.5, 0.0, 0.5);
	pos2 = glm::vec3(-0.5, 0.0, -0.5);
	pos3 = glm::vec3(0.5, 0.0, -0.5);
	pos4 = glm::vec3(0.5, 0.0, 0.5);

	// texture coordinates
	glm::vec2 uv1(0.0, 1.0);
	glm::vec2 uv2(0.0, 0.0);
	glm::vec2 uv3(1.0, 0.0);
	glm::vec2 uv4(1.0, 1.0);
	// normal vector
	glm::vec3 nm(0.0, 1.0, 0.0);
#endif
	// calculate tangent/bitangent vectors of both triangles
	/*glm::vec3 tangent1, bitangent1;
	glm::vec3 tangent2, bitangent2;*/
	// - triangle 1
	glm::vec3 edge1 = pos2 - pos1;
	glm::vec3 edge2 = pos3 - pos1;
	glm::vec2 deltaUV1 = uv2 - uv1;
	glm::vec2 deltaUV2 = uv3 - uv1;

	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent1 = glm::normalize(tangent1);

	bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	bitangent1 = glm::normalize(bitangent1);

	// - triangle 2
	edge1 = pos3 - pos1;
	edge2 = pos4 - pos1;
	deltaUV1 = uv3 - uv1;
	deltaUV2 = uv4 - uv1;

	f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent2 = glm::normalize(tangent2);


	bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	bitangent2 = glm::normalize(bitangent2);
}

//void Util::CreateTBVertex(float* vertexArray) {
//#if 1
//	glm::vec3 pos1(-0.5, 0.5, 0.0);
//	glm::vec3 pos2(-0.5, -0.5, 0.0);
//	glm::vec3 pos3(0.5, -0.5, 0.0);
//	glm::vec3 pos4(0.5, 0.5, 0.0);
//#else
//	glm::vec3 pos1(-0.5, 0.0, 0.5);
//	glm::vec3 pos2(-0.5, 0.0, -0.5);
//	glm::vec3 pos3(0.5, 0.0, -0.5);
//	glm::vec3 pos4(0.5, 0.0, 0.5);
//#endif
//	// texture coordinates
//	glm::vec2 uv1(0.0, 1.0);
//	glm::vec2 uv2(0.0, 0.0);
//	glm::vec2 uv3(1.0, 0.0);
//	glm::vec2 uv4(1.0, 1.0);
//	// normal vector
//	glm::vec3 nm(0.0, 0.0, 1.0);
//
//	// calculate tangent/bitangent vectors of both triangles
//	glm::vec3 tangent1, bitangent1;
//	glm::vec3 tangent2, bitangent2;
//	// - triangle 1
//	glm::vec3 edge1 = pos2 - pos1;
//	glm::vec3 edge2 = pos3 - pos1;
//	glm::vec2 deltaUV1 = uv2 - uv1;
//	glm::vec2 deltaUV2 = uv3 - uv1;
//
//	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
//
//	tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
//	tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
//	tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
//	tangent1 = glm::normalize(tangent1);
//
//	bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
//	bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
//	bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
//	bitangent1 = glm::normalize(bitangent1);
//
//	// - triangle 2
//	edge1 = pos3 - pos1;
//	edge2 = pos4 - pos1;
//	deltaUV1 = uv3 - uv1;
//	deltaUV2 = uv4 - uv1;
//
//	f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
//
//	tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
//	tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
//	tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
//	tangent2 = glm::normalize(tangent2);
//
//
//	bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
//	bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
//	bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
//	bitangent2 = glm::normalize(bitangent2);
//	
//	auto tbVertices=new float[84]
//	{
//		// Positions            // normal         // TexCoords  // Tangent                          // Bitangent
//			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
//			pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
//			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
//
//			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
//			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
//			pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
//	};
//	memcpy(vertexArray, tbVertices, 84 * sizeof(float));
//}

void Util::CreatePlaneVertex(std::vector<glm::vec3>& vertex, std::vector<glm::vec2>& tex, std::vector<size_t>& indices,
	size_t rowLen, size_t colLen) {
	size_t triangleNum = (rowLen - 1) * (colLen - 1) * 2;
	double d = 2.0 / rowLen;
	printf("delta: %lf\n", d);
	for (double z = -1.0; z < 1.0; z+=d) {
		for (double x = -1.0; x < 1.0; x+=d) {
			vertex.push_back(glm::vec3(x, -0.3, z));
			tex.push_back(glm::vec2((x + 1.0) * 0.5, (z + 1.0) * 0.5));
		}
	}
	indices.resize(triangleNum * 3);
	size_t k = 0;
	for (size_t col = 0; col < colLen-1; col++) {
		for (size_t row = 0; row < rowLen-1; row++) {
			// 1,2,3
			indices[k] = col * rowLen + row;
			indices[k + 1] = col * rowLen + row + 1;
			indices[k + 2] = (col + 1) * rowLen + row;
			// 3,2,4
			indices[k + 3] = (col + 1) * rowLen + row;
			indices[k + 4] = col * rowLen + row + 1;
			indices[k + 5] = (col + 1) * rowLen + row + 1;
			k += 6;
		}
	}
}
void Util::CreatePlaneVertex(std::vector<glm::vec3>& vertex, std::vector<glm::vec2>& tex, std::vector<TBvertex>& tbvertex,
	size_t rowLen, size_t colLen) {
	std::vector<glm::vec3> tv;
	std::vector<glm::vec2> te;
	size_t triangleNum = (rowLen - 1) * (colLen - 1) * 2;
	double d = 2.0 / rowLen;
	printf("delta: %lf\n", d);
	for (double z = -1.0; z < 1.0; z += d) {
		for (double x = -1.0; x < 1.0; x += d) {
			tv.push_back(glm::vec3(x, 0.0, z));
			te.push_back(glm::vec2((x + 1.0) * 0.5, (z + 1.0) * 0.5));
		}
	}
	glm::vec3 tangent1, bitangent1;
	glm::vec3 tangent2, bitangent2; 
	CreateTBVertex(tangent1, tangent2, bitangent1, bitangent2);
	vertex.resize(triangleNum * 3);
	tex.resize(triangleNum * 3);
	tbvertex.resize(triangleNum * 3);
	size_t k = 0;
	for (size_t col = 0; col < colLen - 1; col++) {
		for (size_t row = 0; row < rowLen - 1; row++) {
			// 1,2,3
			vertex[k] = tv[col * rowLen + row];				tex[k] = te[col * rowLen + row];			tbvertex[k] = TBvertex{ tangent1,bitangent1 };
			vertex[k + 1] = tv[col * rowLen + row + 1];		tex[k + 1] = te[col * rowLen + row + 1];	tbvertex[k+1] = TBvertex{ tangent1,bitangent1 };
			vertex[k + 2] = tv[(col + 1) * rowLen + row];	tex[k + 2] = te[(col + 1) * rowLen + row];	tbvertex[k+2] = TBvertex{ tangent1,bitangent1 };

			vertex[k + 3] = tv[(col + 1) * rowLen + row];	tex[k + 3] = te[(col + 1) * rowLen + row];	tbvertex[k+3] = TBvertex{ tangent2,bitangent2 };
			vertex[k + 4] = tv[col * rowLen + row + 1];		tex[k + 4] = te[col * rowLen + row + 1];	tbvertex[k+4] = TBvertex{ tangent2,bitangent2 };
			vertex[k + 5] = tv[(col + 1) * rowLen + row + 1]; tex[k + 5] = te[(col + 1) * rowLen + row + 1]; tbvertex[k+5] = TBvertex{ tangent2,bitangent2 };

			k += 6;
		}
	}
}
float* Util::GetPoint(float u, float v, float r) {
	constexpr float pi = glm::pi<float>();
	float z = r * std::cos(pi * u);
	float x = r * std::sin(pi * u) * std::cos(2 * pi * v);
	float y = r * std::sin(pi * u) * std::sin(2 * pi * v);
	float* uni = new float[5]{ x,y,z,u,v};
	//float *uni=new float[8]{ x,y,z,u,v,-x,-y,-z };
	//printf("%f\n", sqrtf(r * r - z * z));
	return uni;
}
void Util::CreatSphereVertex(std::vector<float>& vertex, size_t longtitude, size_t latitude, float r) {
	float longtitude_step = 1.0f / longtitude;
	float latitude_step = 1.0f / latitude;
	size_t offset = 0;
	for (size_t lo = 0; lo < longtitude; lo++) {
		for (size_t lat = 0; lat < latitude; lat++) {
			/*glm::vec3 point1 = GetPoint(lo * longtitude_step, lat * latitude_step, r);
			glm::vec3 point2 = GetPoint((lo + 1) * longtitude_step, lat * latitude_step, r);
			glm::vec3 point3 = GetPoint((lo + 1) * longtitude_step, (lat + 1) * latitude_step, r);
			glm::vec3 point4 = GetPoint(lo * longtitude_step, (lat + 1) * latitude_step, r);*/
			
			float* point1 = GetPoint(lo * longtitude_step, lat * latitude_step, r);
			float* point2 = GetPoint((lo + 1) * longtitude_step, lat * latitude_step, r);
			float* point3 = GetPoint((lo + 1) * longtitude_step, (lat + 1) * latitude_step, r);
			float* point4 = GetPoint(lo * longtitude_step, (lat + 1) * latitude_step, r);

			for (size_t i = 0; i < 5; ++i) 
				vertex.push_back(point1[i]);
			for (size_t i = 0; i < 5; ++i)
				vertex.push_back(point3[i]);
			for (size_t i = 0; i < 5; ++i)
				vertex.push_back(point2[i]);
			for (size_t i = 0; i < 5; ++i)
				vertex.push_back(point1[i]);
			for (size_t i = 0; i < 5; ++i)
				vertex.push_back(point4[i]);
			for (size_t i = 0; i < 5; ++i)
				vertex.push_back(point3[i]);
		}
	}
}

int Util::sign(double k) {
	if (k > 0.0)return 1;
	else if (k == 0.0) return 0;
	else if (k < 0.0)return -1;
}
glm::mat4 Util::GetReflectMatrix(glm::vec3 normal, glm::vec3 p) {
	glm::mat4 result = glm::mat4(1.0);
	float d = -glm::dot(normal, p);
	//printf("d = %f\n", d);
	result[0][0] = 1.0 - 2 * normal.x * normal.x;
	result[0][1] = -2 * normal.x * normal.y;
	result[0][2] = -2 * normal.x * normal.z;
	result[0][3] = -2 * normal.x * d;

	result[1][0] = -2 * normal.x * normal.y;
	result[1][1] =  1.0 - 2 * normal.y * normal.y;
	result[1][2] = -2 * normal.y * normal.z;
	result[1][3] = -2 * normal.y * d;

	result[2][0] = -2 * normal.x * normal.z;
	result[2][1] = -2 * normal.z * normal.y;
	result[2][2] = 1 - 2 * normal.z * normal.z;
	result[2][3] = -2 * normal.z * d;

	result[3][0] = 0;
	result[3][1] = 0;
	result[3][2] = 0;
	result[3][3] = 1;

	return result;
}
glm::mat4 Util::CaculateObliqueViewFrustumMatrix(glm::mat4 projection, glm::mat4 reflectView, glm::vec4 plane) {
	//glm::mat4 invM = glm::inverse(reflectView);
	glm::vec4 viewSpacePlane = glm::transpose(glm::inverse(reflectView)) * plane;
	glm::vec4 ViewSpaceFraPlanePoint = glm::transpose(glm::inverse(projection))
		* glm::vec4(sign(viewSpacePlane.x), sign(viewSpacePlane.y), 1, 1);
	glm::vec4 M4 = glm::vec4(projection[3][0], projection[3][1], projection[3][2], projection[3][3]);
	auto u = 2.0f * (glm::dot(M4,ViewSpaceFraPlanePoint) / glm::dot(ViewSpaceFraPlanePoint, viewSpacePlane));
	auto newViewSpaceNearPlane = u * viewSpacePlane;
	auto M3 = newViewSpaceNearPlane - M4;
	//glm::vec4 M3 = 2.0f * (glm::dot(M4, ViewSpaceFraPlanePoint) / glm::dot(plane, ViewSpaceFraPlanePoint) * C) - M4;
	projection[0][2] = M3.x;
	projection[1][2] = M3.y;
	projection[2][2] = M3.z;
	projection[3][2] = M3.w;
	return projection;
}