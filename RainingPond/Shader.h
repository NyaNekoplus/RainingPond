#ifndef SHADER_H_
#define SHADER_H_

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace OpenGL {
	class Shader {
	public:
		Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
		Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath);
		unsigned int ID;//³ÌÐòid
		
		void use()const;//¼¤»îglprogram
		//uniform tool function
		void setBool(const std::string& name, bool value)const;
		void setInt(const std::string& name, int value)const;
		void setFloat(const std::string& name, float value)const;
		void setVec3(const std::string& name, float x, float y, float z)const;
		void setVec3(const std::string& name, glm::vec3 pos)const;
		void setMat4(const std::string& name, int count, bool isTranspose, const GLfloat* value)const;
		void setMat4(const std::string& name, const glm::mat4 value)const;

	};
}

#endif