#ifndef FBO_H_
#define FBO_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Texture.h"
#include "util.h"
#include "Ptr.h"

namespace OpenGL {
	class FBO {
	public:
		enum TYPE {
			TYPE_2D,
			TYPE_2D_LUT,
			TYPE_CUBEMAP,
			TYPE_CUBEMAP_FILTER,
			TYPE_GBUFFER
		};
		FBO(size_t width, size_t height, TYPE type, int precision=1);
		void Bind();
		void UnBind();
		void Blit();
		bool SetBuffer(size_t index, size_t mip = 0);
		bool ReSize(size_t width, size_t height);
		bool IsComplete();
		size_t GetID()const;
		size_t GetRBO();
		const Basic::Ptr<Texture> GetTexture()const;
		
	private:
		size_t ID;
		size_t rboID;
		size_t width;
		size_t height;
		TYPE type;
		std::vector<Texture> colorTexture;
		bool IsValid;
		bool GenFBO(size_t w, size_t h);
		bool GenFBO_2D(size_t w, size_t h, int precision);
		bool GenFBO_CUBE(size_t w, size_t h, bool isPreFilterMap);
		bool GenFBO_GBUFFER(size_t w, size_t h);
	};
}

#endif // !FBO_H_
