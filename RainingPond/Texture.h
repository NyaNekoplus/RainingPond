#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <glad/glad.h>
#include <string>
#include <vector>

#include "util.h"

namespace OpenGL {
	class Texture {
	public:
		enum TYPE
		{
			TYPE_INVAILD,
			TYPE_CUBEMAP,
			TYPE_2D
		};
		Texture(size_t ID, TYPE type);
		Texture(size_t width, size_t height, bool isCubeMap, bool isPreFilterMap);
		Texture(const std::string& fname, bool imageFilp=false, bool isHDR = false);
		Texture(const std::vector<std::string> skybox);

		//Texture(bool isCubeMap=false);
		void UnBind();
		size_t GetID()const;
		void Use(size_t id = 0)const;

		static const Texture Invalid;
	private:
		TYPE type;
		size_t GetType()const;
		size_t ID;
	};
}

#endif