#include "Texture.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace OpenGL;

const Texture Texture::Invalid(0, TYPE_INVAILD);

Texture::Texture(size_t id, TYPE type) {
	this->ID = id;
	this->type = type;
}
Texture::Texture(const std::string& name, bool imageFilp ,bool isHDR) {
	glGenTextures(1, &ID);
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(imageFilp); 
	void* data;
	if (isHDR)
		data = stbi_loadf(name.c_str(), &width, &height, &nrChannels, 0);
	else
		data = stbi_load(name.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		GLenum internalFormat, dataFormat, dataType=0;
		if (nrChannels == 1) {
			internalFormat=dataFormat = GL_RED;
		}	
		else if (nrChannels == 3) {
			internalFormat = isHDR ? GL_RGB16F : GL_RGB;
			dataType = isHDR ? GL_FLOAT : GL_UNSIGNED_BYTE;
			dataFormat = GL_RGB;
		}
		else if (nrChannels == 4) {
			internalFormat = isHDR ? GL_RGBA16F : GL_RGBA;
			dataType = isHDR ? GL_FLOAT : GL_UNSIGNED_BYTE;
			dataFormat = GL_RGBA;
		}
		//std::cout << internalFormat << " " << dataFormat << " " << dataType << std::endl;
		glBindTexture(GL_TEXTURE_2D, ID);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, dataType, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		if(isHDR)
			std::cout << "Failed to load HDR:" << name << std::endl;
		else
			std::cout << "Failed to load texture:" <<name<< std::endl;
	}
	stbi_image_free(data);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(false);
	type = TYPE_2D;
	UnBind();
}

Texture::Texture(const std::vector<std::string> skybox) {
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
	int w, h, nrChannels;
	unsigned char* data;
	for (size_t i = 0; i < skybox.size();++i) {
		data = stbi_load(skybox[i].c_str(), &w, &h, &nrChannels, 0);
		if (data) {
			GLenum format;
			if (nrChannels == 1)format = GL_RED;
			else if (nrChannels == 3)format = GL_RGB;
			else if (nrChannels == 4)format = GL_RGBA;

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//x
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//y
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);//z
	type = TYPE_CUBEMAP;
	UnBind();
}

Texture::Texture(size_t width, size_t height, bool isCubeMap, bool isPrefilterMap) {
	glGenTextures(1, &ID);
	if (isCubeMap) {
		auto filter = isPrefilterMap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
		for (size_t i = 0; i < 6; ++i) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//x
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//y
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);//z
		if(isPrefilterMap)
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		type = TYPE_CUBEMAP;
		std::cout << "ря╪сть:" <<ID<< width<<height<<std::endl;
	}
	else {
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, width, width, 0, GL_RG, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		type = TYPE_2D;
	}
	UnBind();
}
size_t Texture::GetID()const { return ID; }
size_t Texture::GetType()const {
	switch (type)
	{
	case OpenGL::Texture::TYPE_INVAILD:
		return NULL;
	case OpenGL::Texture::TYPE_CUBEMAP:
		return GL_TEXTURE_CUBE_MAP;
	case OpenGL::Texture::TYPE_2D:
		return GL_TEXTURE_2D;
	default:
		return NULL;
	}
}
void Texture::UnBind() {
	glBindTexture(GetType(), 0);
}
void Texture::Use(size_t id)const {
	glActiveTexture(GL_TEXTURE0 + id);
	glBindTexture(GetType(), ID);
}