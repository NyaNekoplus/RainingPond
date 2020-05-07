#include "VAO.h"

using namespace OpenGL;
VAO::VAO(const float* data, size_t dataSize, const std::vector<size_t>& attrCount) {
	if (data == NULL || attrCount.empty() || dataSize == 0) {
		isValid = false;
		ID = 0;
		return;
	}
	glGenVertexArrays(1,&ID);
	glBindVertexArray(ID);

	size_t VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
	
	size_t increment=0;
	for (auto i : attrCount)
		increment += i;
	for (size_t i = 0,offset=0; i < attrCount.size(); offset+=attrCount[i],++i) {
		glVertexAttribPointer(i, attrCount[i], GL_FLOAT, GL_FALSE, increment * sizeof(float), (void*)(offset * sizeof(float)));
		glEnableVertexAttribArray(i);
	}
	this->hasIndex = false;
	PointNum = dataSize / (increment * sizeof(float));
	printf("数据大小：%d\n", dataSize);
	printf("点数：%d\n", PointNum);
}

VAO::VAO(const float* data, size_t dataSize, const std::vector<size_t>& attrCount, const size_t* indices, const size_t indiceSize)
	:VAO(data,dataSize,attrCount)
{
	if (IsValid()) 
		isValid=BindEBO(indices, indiceSize);
}

VAO::VAO(const std::vector<glm::vec3>& vertexData,
		const std::vector<glm::vec2>& texData) {
	if (vertexData.size()==0){
		isValid = false;
		ID = 0;
		return;
	}
	glGenVertexArrays(1, &ID);
	glBindVertexArray(ID);
	
	size_t VBO[2];
	glGenBuffers(2, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);//vertex
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(glm::vec3), vertexData.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);//texcoord
	glBufferData(GL_ARRAY_BUFFER, texData.size() * sizeof(glm::vec2), texData.data(), GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);//vertex
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);//texcoord
	glEnableVertexAttribArray(1);
	
	hasIndex = false;
	PointNum = vertexData.size();
	printf("点数：%d\n", PointNum);
}
VAO::VAO(const std::vector<glm::vec3>& vertexData,
	const std::vector<glm::vec2>& texData,
	const std::vector<size_t>& indices):VAO(vertexData,texData) {
	if (IsValid())
		isValid = BindEBO(indices);
}
VAO::VAO(const std::vector<glm::vec3>& vertexData,
	const std::vector<glm::vec2>& texData,
	const std::vector<Util::TBvertex>& TBvertex) {
	if (vertexData.size() == 0) {
		isValid = false;
		ID = 0;
		return;
	}
	glGenVertexArrays(1, &ID);
	glBindVertexArray(ID);

	size_t VBO[3];
	glGenBuffers(3, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);//vertex
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(glm::vec3), vertexData.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);//texcoord
	glBufferData(GL_ARRAY_BUFFER, texData.size() * sizeof(glm::vec2), texData.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);// tangent and bitangent
	glBufferData(GL_ARRAY_BUFFER, TBvertex.size() * sizeof(Util::TBvertex), TBvertex.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);//vertex
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);//texcoord
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Util::TBvertex), (void*)0);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Util::TBvertex), (void*)(sizeof(glm::vec3)));
	glEnableVertexAttribArray(3);
	hasIndex = false;
	PointNum = vertexData.size();
	printf("点数：%d\n", PointNum);
}

bool VAO::BindEBO(const size_t* indices, const size_t dataSize) {
	size_t EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, indices, GL_STATIC_DRAW);
	hasIndex = true;
	PointNum = dataSize / sizeof(size_t);
	return true;
}
bool VAO::BindEBO(const std::vector<size_t>& indices) {
	size_t EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(size_t), indices.data(), GL_STATIC_DRAW);
	hasIndex = true;
	PointNum = indices.size();
	return true;
}

bool VAO::IsValid()const {
	return this->isValid;
}

bool VAO::Bind()const {
	if (IsValid()) {
		glBindVertexArray(ID);
		return true;
	}
	else {
		std::cout << "Invalid VAO,bind failed" << std::endl;
		return false;
	}
}

bool VAO::Draw()const {
	if (!Bind()) {
		printf("VAO Not yet bound. \n");
		return false;
	}
		
	if (hasIndex) {
		glDrawElements(GL_TRIANGLES, PointNum, GL_UNSIGNED_INT, NULL);
		//printf("%d", PointNum);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, PointNum);
		//printf("%d", PointNum);
	}
	return true;
}

size_t VAO::GetID()const {	return ID;}