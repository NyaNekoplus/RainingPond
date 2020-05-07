#include "FBO.h"

using namespace OpenGL;

FBO::FBO(size_t w, size_t h, TYPE type, int precision): width(w),height(h),type(type){
	switch (type)
	{
	case OpenGL::FBO::TYPE_2D:
		GenFBO_2D(w, h, precision);
		break;
	case OpenGL::FBO::TYPE_2D_LUT:
		GenFBO_2D(w, h, precision);
		break;
	case OpenGL::FBO::TYPE_CUBEMAP:
		GenFBO_CUBE(w, h, false);
		break;
	case OpenGL::FBO::TYPE_CUBEMAP_FILTER:
		GenFBO_CUBE(w, h, true);
		break;
	case OpenGL::FBO::TYPE_GBUFFER:
		GenFBO_CUBE(w, h, true);
		break;
	default:
		printf("FBO Type Error!\n");
		break;
	}
}

bool FBO::GenFBO(size_t w, size_t h) {
	glGenFramebuffers(1, &ID);
	glGenRenderbuffers(1, &rboID);

	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	glBindRenderbuffer(GL_RENDERBUFFER, rboID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, w, h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboID);
	//UnBind();
	

	IsValid = IsComplete();
	return IsValid ? true : false;
}
bool FBO::GenFBO_2D(size_t w, size_t h, int precision) {
	size_t textureID;
	GLuint dataFormat = GL_RGB16F, internalFormat = GL_RGB;
	if (precision == 0) {
		internalFormat = GL_RG;
		dataFormat = GL_RG16F;
	}
	else if (precision == 1) {
		internalFormat = GL_RGB;
		dataFormat = GL_RGB16F;
	}
	else if (precision == 2) {
		internalFormat = GL_RGB;
		dataFormat = GL_RGB32F;
	}
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, dataFormat,	w, h, 0, internalFormat, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &ID);
	glGenRenderbuffers(1, &rboID);

	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	glBindRenderbuffer(GL_RENDERBUFFER, rboID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, w, h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboID);

	colorTexture.push_back(Texture(textureID, Texture::TYPE_2D));

	IsValid = IsComplete();
	return IsValid ? true : false;
}
bool FBO::GenFBO_CUBE(size_t w, size_t h, bool isPreFilterMap) {
	auto filter = isPreFilterMap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;

	size_t textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (size_t i = 0; i < 6; ++i) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGB16F, w, h, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//x
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//y
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);//z
	if (isPreFilterMap)
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glGenFramebuffers(1, &ID);
	glBindFramebuffer(GL_FRAMEBUFFER, ID);

	glGenRenderbuffers(1, &rboID);
	glBindRenderbuffer(GL_RENDERBUFFER, rboID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, w, h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboID);

	colorTexture.push_back(Texture(textureID, Texture::TYPE_CUBEMAP));

	IsValid = IsComplete();
	return IsValid ? true : false;
}
bool FBO::GenFBO_GBUFFER(size_t width, size_t height) {
	glGenFramebuffers(1, &ID);
	glBindFramebuffer(GL_FRAMEBUFFER, ID);

	GLuint gPosition, gAlbedo, gNormal;

	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	glGenTextures(1, &gAlbedo);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gAlbedo, 0);

	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gNormal, 0);

	GLuint attachments[] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,
							GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);
	// - Create and attach depth buffer (renderbuffer)
	glGenRenderbuffers(1, &rboID);
	glBindRenderbuffer(GL_RENDERBUFFER, rboID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboID);

	colorTexture.push_back(Texture(gPosition, Texture::TYPE_2D));
	colorTexture.push_back(Texture(gAlbedo, Texture::TYPE_2D));
	colorTexture.push_back(Texture(gNormal, Texture::TYPE_2D));

	IsValid = IsComplete();
	return IsValid ? true : false;
}
void FBO::Bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
}
void FBO::UnBind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void FBO::Blit() {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, ID);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);// Write to default framebuffer
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
bool FBO::SetBuffer(size_t index, size_t mip) {
	if (!IsValid) {
		printf("ERROR::FBO::SetBuffer:\n"
			"\t""FBO is invalid\n");
		return false;
	}
	size_t target[]{	GL_TEXTURE_CUBE_MAP_POSITIVE_X,
						GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
						GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
						GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
						GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
						GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
						GL_TEXTURE_2D };
	Bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		target[index], colorTexture[0].GetID(), mip);
	return true;
}
bool FBO::ReSize(size_t width, size_t height) {
	glBindRenderbuffer(GL_RENDERBUFFER, rboID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	return true;
}
bool FBO::IsComplete() {
	//glBindFramebuffer(GL_FRAMEBUFFER, ID);
	auto rst = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (rst != GL_FRAMEBUFFER_COMPLETE) {
		printf("Framebuffer is not complete!\n");
		return false;
	}

	return true;
}
const Basic::Ptr<Texture> FBO::GetTexture()const {
	if (!IsValid)
		return std::make_shared<Texture>(Texture::Invalid);
	if (type != TYPE_CUBEMAP && type != TYPE_2D)
		return std::make_shared<Texture>(Texture::Invalid);
	return std::make_shared<Texture>(colorTexture[0]);
}
size_t FBO::GetID()const { return this->ID; }
size_t FBO::GetRBO() { return this->rboID; }