#include <iostream>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLfw.h"
#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
#include "VAO.h"
#include "FBO.h"

#include "Queue.h"

#include "util.h"
#include "vertex.h"
struct Ame {
	glm::vec3 position;
	float extent;
	float time;
};
struct Ripple {
	glm::vec3 position;
	float time;
};

void RenderRain(Basic::Ptr<Basic::Queue<Ame>> rain, Basic::Ptr<Basic::Queue<Ripple>> ripple,
	const Basic::Ptr<OpenGL::Shader> shader, const Basic::Ptr<OpenGL::VAO> vao,
	glm::mat4 v, glm::mat4 proj, bool PositionUpdate);
void RenderSkyBox(const Basic::Ptr<OpenGL::Shader> shader, const Basic::Ptr<OpenGL::Texture> tex, const Basic::Ptr<OpenGL::VAO> vao, glm::mat4 v, glm::mat4 proj);
void RenderWater(const Basic::Ptr<OpenGL::Shader> shader,
	const Basic::Ptr<OpenGL::Texture> reflectTex, const Basic::Ptr<OpenGL::Texture> refractTex, const Basic::Ptr<OpenGL::Texture> heightTex, const Basic::Ptr<OpenGL::Texture> rippleTex,
	const Basic::Ptr<OpenGL::VAO> vao, glm::mat4 v, glm::mat4 proj);
void RenderPlane(const Basic::Ptr<OpenGL::Shader> shader,
	const std::vector<Basic::Ptr<OpenGL::Texture>> tex,
	const Basic::Ptr<OpenGL::VAO> vao, glm::mat4 v, glm::mat4 proj);

int main(void) {
	Basic::Ptr<Basic::Queue<Ame>> rain = std::make_shared< Basic::Queue<Ame>>(256);
	auto ripple = std::make_shared < Basic::Queue <Ripple>>(256);
	////auto p = new Basic::Queue<float>();
	//Ame t;
	//for (int i = 0; i < 10; ++i) {
	//	t.time = 0.21+i;
	//	rain->Push(t);
	//}
	//while (rain->Pop(t)) {
	//	std::cout << t.time << std::endl;
	//}
	Ame data;
	//for (int h = -10; h < 10; h++) {
	//	for (int w = -10; w < 10; w++) {
	//		glm::vec3 pos(h + 0.9 * Util::drand48(), 4.0f + Util::drand48() * 100, w + 0.9 * Util::drand48());
	//		data.position = pos;
	//		data.time = 1.0f;
	//		data.extent = Util::MinusOneToOne() * 0.05;
	//		rain->Push(data);
	//	}
	//}
	for (size_t i = 0; i < rain->Capcity(); i++) {
		glm::vec3 rainPos(Util::PLANE_SIZE* Util::MinusOneToOne(), Util::drand48() * 100, Util::PLANE_SIZE * Util::MinusOneToOne());
		data.position = rainPos;
		data.time = 0.001f;
		data.extent = Util::MinusOneToOne() * 0.05;
		rain->Push(data);
	}
	

	std::vector<glm::vec3> planeVertices;
	std::vector<glm::vec2> planeTexcoord;
	std::vector<Util::TBvertex> planeTBvertex;
	Util::CreatePlaneVertex(planeVertices, planeTexcoord, planeTBvertex, 1024, 1024);
	//std::vector<float> total,wtotal;
	//for (int i = 0; i < planeVertices.size(); ++i) {
	//	total.push_back(planeVertices[i].x);
	//	total.push_back(planeVertices[i].y);
	//	total.push_back(planeVertices[i].z);
	//	total.push_back(planeTexcoord[i].x);
	//	total.push_back(planeTexcoord[i].y);
	//	total.push_back(planeTBvertex[i].tangent.x);
	//	total.push_back(planeTBvertex[i].tangent.y);
	//	total.push_back(planeTBvertex[i].tangent.z);
	//	total.push_back(planeTBvertex[i].bitangent.x);
	//	total.push_back(planeTBvertex[i].bitangent.y);
	//	total.push_back(planeTBvertex[i].bitangent.z);
	//}
	//for (int i = 0; i < planeVertices.size(); ++i) {
	//	wtotal.push_back(planeVertices[i].x);
	//	wtotal.push_back(planeVertices[i].y);
	//	wtotal.push_back(planeVertices[i].z);
	//	wtotal.push_back(planeTexcoord[i].x);
	//	wtotal.push_back(planeTexcoord[i].y);
	//}
	//printf("%lf,%lf,%lf\n", pv[16383].x, pv[0].y, pv[16383].z);
	//printf("vertex size: %d\n", pv.size()); 
	//printf("indice size: %d\n", planeIndices.size());

	
	
	OpenGL::GLfw::GetInstance()->Init();
	GLFWwindow* window = OpenGL::GLfw::GetInstance()->GetWindow();

	GLint max;
	glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &max);
	std::cout << max << std::endl;
	auto testShader = std::make_shared<OpenGL::Shader>("test.vs", "test.fs");

	auto planeShader = std::make_shared<OpenGL::Shader>("pool.vs", "pool.fs");
	auto waterShader = std::make_shared<OpenGL::Shader>("water.vs", "water.fs");
	auto rippleShader = std::make_shared<OpenGL::Shader>("ripple.vs", "ripple.fs");
	auto skyboxShader = std::make_shared<OpenGL::Shader>("skybox.vs", "skybox.fs");
	auto rainShader = std::make_shared<OpenGL::Shader>("rain.vs", "rain.fs");
	auto equirectangularToCubemapShader = std::make_shared<OpenGL::Shader>("hdr.vs", "hdr.fs");
	auto irradianceShader = std::make_shared<OpenGL::Shader>("hdr.vs", "irradiance.fs");
	auto preFilterShader = std::make_shared<OpenGL::Shader>("hdr.vs", "preFilter.fs");
	auto brdfShader = std::make_shared<OpenGL::Shader>("brdf.vs", "brdf.fs");

	auto hdrTex = std::make_shared<OpenGL::Texture>("../texture/Ice_Lake_Ref.hdr", true, true);
	//OpenGL::Texture envCubeMap(512, 512, true,false);							//天空盒
	//OpenGL::Texture irradianceMap(32, 32, true,false);						//卷积辐照度图
	//OpenGL::Texture preFilterMap(128, 128, true, true);
	//OpenGL::Texture brdfLUT(512, 512, false, false);
	auto planeNormalMap = std::make_shared<OpenGL::Texture>("../texture/tidal-pool2/normal.png", true, false);
	auto planeAlbedoMap = std::make_shared<OpenGL::Texture>("../texture/tidal-pool2/albedo.png", true, false);
	auto planeAOMap = std::make_shared<OpenGL::Texture>("../texture/tidal-pool2/ao.png", true, false);//Ambient_Occlusion
	auto planeDepthMap = std::make_shared<OpenGL::Texture>("../texture/tidal-pool2/height1024.png", true, false);
	auto planeMetallicMap = std::make_shared<OpenGL::Texture>("../texture/tidal-pool2/metallic.png", true, false);
	auto planeRoughMap = std::make_shared<OpenGL::Texture>("../texture/tidal-pool2/roughness2.png", true, false);

	auto rippleTest = std::make_shared<OpenGL::Texture>("../texture/jagged-rockface1/height.png", true, false);
	//OpenGL::Texture reflectMap(Util::SCR_WIDTH,Util::SCR_HEIGHT,)
	//glEnable(GL_MULTISAMPLE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);//在立方体贴图的面之间进行正确的过滤
	glDepthFunc(GL_LEQUAL);
	
	
	auto planeVAO = std::make_shared<OpenGL::VAO>(planeVertices, planeTexcoord, planeTBvertex);
	std::vector<float> rainVertex;
	std::vector<size_t> rainAttr{ 3,2 };
	std::vector<size_t> skyAttr{ 3 };
	std::vector<size_t> Attr{ 3,2 };
	Util::CreatSphereVertex(rainVertex, 10, 20, 0.05f);
	auto rainVAO = std::make_shared<OpenGL::VAO>(rainVertex.data(), rainVertex.size()*sizeof(float), rainAttr);
	auto skyVAO = std::make_shared<OpenGL::VAO>(skyboxVertices, sizeof(skyboxVertices), skyAttr);
	auto brdflutVAO = std::make_shared<OpenGL::VAO>(planeVertice, sizeof(planeVertice), Attr);
	std::vector<glm::vec3> waterVertices{ planeVertices };
	std::vector<glm::vec2> waterTexcoord{ planeTexcoord };

	//auto waterVAO(waterVertices, waterTexcoord);
	auto waterVAO = std::make_shared<OpenGL::VAO>(waterVertices, waterTexcoord);
	auto rippleVAO = std::make_shared<OpenGL::VAO>(waterVertices, waterTexcoord);
	auto testVAO = std::make_shared<OpenGL::VAO>(screenVertex, sizeof(screenVertex), Attr, indices, sizeof(indices));

	auto hdrFBO = std::make_shared<OpenGL::FBO>(512, 512, OpenGL::FBO::TYPE_CUBEMAP);
	auto irradianceFBO = std::make_shared<OpenGL::FBO>(32, 32, OpenGL::FBO::TYPE_CUBEMAP);
	auto preFilterFBO = std::make_shared<OpenGL::FBO>(128, 128, OpenGL::FBO::TYPE_CUBEMAP);
	auto brdfFBO = std::make_shared<OpenGL::FBO>(512, 512, OpenGL::FBO::TYPE_2D_LUT, 0);
	
	auto reflectFBO = std::make_shared<OpenGL::FBO>(Util::SCR_WIDTH, Util::SCR_HEIGHT, OpenGL::FBO::TYPE_2D);
	auto refractFBO	= std::make_shared<OpenGL::FBO>(Util::SCR_WIDTH,Util::SCR_HEIGHT,OpenGL::FBO::TYPE_2D);
	auto rippleFBO = std::make_shared<OpenGL::FBO>(Util::SCR_WIDTH, Util::SCR_HEIGHT, OpenGL::FBO::TYPE_2D, 2);//RGB32F

	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};
	equirectangularToCubemapShader->use();
	equirectangularToCubemapShader->setInt("equirectangularMap", 0);
	equirectangularToCubemapShader->setMat4("projection", captureProjection);
	hdrTex->Use();
	glViewport(0, 0, 512, 512);
	hdrFBO->Bind();
	for (size_t i = 0; i < 6; ++i) {
		equirectangularToCubemapShader->setMat4("view", captureViews[i]);
		hdrFBO->SetBuffer(i);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skyVAO->Bind();
		skyVAO->Draw();//skyboxShader
		glBindVertexArray(0);
	}
	hdrFBO->UnBind();
	

	irradianceShader->use();
	irradianceShader->setInt("environmentMap", 0);
	irradianceShader->setMat4("projection", captureProjection);
	
	hdrFBO->GetTexture()->Use();
	glViewport(0, 0, 32, 32);
	irradianceFBO->Bind();
	for (size_t i = 0; i < 6; ++i) {
		irradianceShader->setMat4("view", captureViews[i]);
		irradianceFBO->SetBuffer(i);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//!!!!!!!!!!!!!!!!!!!!
		skyVAO->Bind();
		skyVAO->Draw();//skyboxShader
		glBindVertexArray(0);
	}
	irradianceFBO->UnBind();


	preFilterShader->use();
	preFilterShader->setInt("environmentMap", 0);
	preFilterShader->setMat4("projection", captureProjection);

	hdrFBO->GetTexture()->Use();
	preFilterFBO->Bind();
	size_t maxMiplevels = 5;
	for (size_t mip = 0; mip < maxMiplevels; ++mip) {
		//每增加一级尺寸缩小一半
		size_t mipWidth = 128 * std::pow(0.5, mip);
		size_t mipHeight = mipWidth;
		preFilterFBO->ReSize(mipWidth, mipHeight);
		glViewport(0, 0, mipWidth, mipHeight);

		float roughness = (float)mip / (float)(maxMiplevels - 1);//层级
		preFilterShader->setFloat("roughness", roughness);
		for (size_t i = 0; i < 6; ++i) {
			preFilterShader->setMat4("view", captureViews[i]);
			preFilterFBO->SetBuffer(i, mip);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			skyVAO->Bind();
			skyVAO->Draw();//skyboxShader
			glBindVertexArray(0);
		}
	}
	preFilterFBO->UnBind();
	
	brdfShader->use();
	brdfFBO->GetTexture()->Use();
	brdfFBO->Bind();
	glViewport(0, 0, 512, 512);
	brdfFBO->SetBuffer(6);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	brdflutVAO->Bind();
	brdflutVAO->Draw();//brdfShader
	glBindVertexArray(0);
	brdfFBO->UnBind();
	
	planeShader->use();
	glm::mat4 planeModel = glm::mat4(1.0f);
	planeModel = glm::scale(planeModel, glm::vec3(30));
	planeModel = glm::translate(planeModel, glm::vec3(0.0f, -0.02f, 0.0f));
	planeShader->setMat4("model", planeModel);
	planeShader->setInt("albedoMap",		0);
	planeShader->setInt("NormalMap",		1);
	planeShader->setInt("irradianceMap", 2);
	planeShader->setInt("aoMap",			3);
	planeShader->setInt("HeightMap",		4);
	planeShader->setInt("metallic",		5);
	planeShader->setInt("roughness",		6);
	planeShader->setInt("prefilterMap",	7);
	planeShader->setInt("brdfLUT",		8);
	planeShader->setVec3("LightPos",		glm::vec3(0.0f, 10.0f, 0.0f));
	planeShader->setVec3("LightColors",	glm::vec3(1000.0f));
	planeShader->setFloat("HeightScale", 0.3f);
	const std::vector<Basic::Ptr<OpenGL::Texture>> planeTex{
	planeAlbedoMap,planeNormalMap,irradianceFBO->GetTexture(),planeAOMap,planeDepthMap,planeMetallicMap,planeRoughMap,preFilterFBO->GetTexture(),brdfFBO->GetTexture()};

	//planeShader.setFloat("metallic", 1.0f);
	//
	glEnable(GL_CULL_FACE);
	glViewport(0, 0, Util::SCR_WIDTH, Util::SCR_HEIGHT);
	glClearColor(0.2f, 0.3f, 0.6f, 1.0f);
	while (!glfwWindowShouldClose(window)) {
		OpenGL::Camera::GetInstance()->CalcFrameDelay(glfwGetTime());
		float delay = OpenGL::Camera::GetInstance()->GetFrameDelay();
		//printf("delay = %f\n", delay);
		glm::mat4 view = OpenGL::Camera::GetInstance()->GetViewMatrix();
		glm::mat4 projection = OpenGL::Camera::GetInstance()->GetProjectionMatrix();
		glm::mat4 orthoproj = OpenGL::Camera::GetInstance()->GetProjectionMatrix(OpenGL::Camera::PROJ_MODE::ORTHONORMAL);

		glCullFace(GL_BACK);
		reflectFBO->Bind();
		reflectFBO->SetBuffer(6);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glm::mat4 reflectView = view * Util::GetReflectMatrix();
		glm::mat4 reflectProj= Util::CaculateObliqueViewFrustumMatrix(projection, view);
		RenderRain(rain, ripple, rainShader, rainVAO, reflectView, reflectProj,false);
		RenderPlane(planeShader, planeTex, planeVAO, reflectView, reflectProj);
		RenderSkyBox(skyboxShader, hdrFBO->GetTexture(), skyVAO, reflectView, reflectProj);
		reflectFBO->UnBind();

		
		glCullFace(GL_FRONT);
		rippleFBO->Bind();
		rippleFBO->SetBuffer(6);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		rippleShader->use();
		rippleShader->setMat4("model", glm::translate(glm::scale(glm::mat4(1.0),glm::vec3(30)),glm::vec3(0.0f)));
		rippleShader->setMat4("view", view);
		rippleShader->setMat4("projection", projection);
		rippleShader->setInt("RippleNum", ripple->Size());
		rippleVAO->Bind();
		Ripple tr;
		size_t size = ripple->Size();
		for (size_t i = 0; i < size; i++) {
			tr = ripple->Pop();
			if (tr.time >= 2.5f) {
				tr.position = glm::vec3(0.0f);
				tr.time = 0.0f;
			}
			if (tr.time > 0.0f) {
				tr.time += delay;
				rippleShader->setVec3("RipplePosition[" + std::to_string(i) + "]", tr.position);
				rippleShader->setFloat("PeriodOfRain", tr.time);
				ripple->Push(tr);
			}
			
		}
		rippleVAO->Draw();//rippleshader
		glBindVertexArray(0);
		rippleFBO->UnBind();


		glDisable(GL_DEPTH_TEST);
		glCullFace(GL_FRONT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		RenderRain(rain, ripple, rainShader, rainVAO, view, projection, true);
		RenderPlane(planeShader, planeTex, planeVAO, view, projection);
		RenderWater(waterShader, reflectFBO->GetTexture(), hdrFBO->GetTexture(), planeDepthMap, rippleFBO->GetTexture(), waterVAO, view, projection);
		RenderSkyBox(skyboxShader, hdrFBO->GetTexture(), skyVAO, view, projection);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glCullFace(GL_BACK);
#if 0
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		testShader->use();
		testShader->setInt("Map", 0);
		testVAO->Bind();
		glDisable(GL_DEPTH_TEST);
		//rippleTest->Use();
		rippleFBO->GetTexture()->Use();
		testVAO->Draw();//testShader
		glBindVertexArray(0);
#endif
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}
void RenderRain(Basic::Ptr<Basic::Queue<Ame>> rain, Basic::Ptr<Basic::Queue<Ripple>> ripple,
	const Basic::Ptr<OpenGL::Shader> shader, const Basic::Ptr<OpenGL::VAO> vao,
	glm::mat4 v, glm::mat4 proj, bool PositionUpdate) {
	Ripple rtemp;
	shader->use();
	shader->setMat4("view", v);
	shader->setMat4("projection", proj);
	vao->Bind();
	size_t size = rain->Size();
	for (size_t j = 0; j < size; ++j) {
		glm::mat4 rainModel = glm::mat4(1.0f);
		Ame temp = rain->Pop();
		rainModel = glm::translate(rainModel, temp.position);
		shader->setMat4("model", rainModel);
		vao->Draw();//rainShader
		if (PositionUpdate) {
			temp.position -= glm::vec3(temp.extent, 0.08f, temp.extent);
			if (temp.position.x > 30.0f || temp.position.x < -30.0f || temp.position.z > 30.0f || temp.position.z < -30.0f)
				temp.extent = -temp.extent;
			if (temp.position.y < -0.1f) {

				rtemp.position = temp.position;
				rtemp.time = temp.time;
				ripple->Push(rtemp);
				temp.position = glm::vec3(Util::MinusOneToOne() * 30, Util::drand48() * 50, Util::MinusOneToOne() * 30);
				temp.time = 0.001;
			}
		}
		rain->Push(temp);
	}
	glBindVertexArray(0);
}

void RenderPlane(const Basic::Ptr<OpenGL::Shader> shader,
	const std::vector<Basic::Ptr<OpenGL::Texture>> tex,
	const Basic::Ptr<OpenGL::VAO> vao, glm::mat4 v, glm::mat4 proj) {
	shader->use();
	shader->setVec3("ViewPos", OpenGL::Camera::GetInstance()->GetPosition());
	shader->setMat4("view", v);
	shader->setMat4("projection", proj);
	vao->Bind();
	for (size_t i = 0; i < tex.size(); i++)
		tex[i]->Use(i);
	/*tex[0]->Use(0);
	tex[1]->Use(1);
	tex[2]->Use(2);
	tex[3]->Use(3);
	tex[4]->Use(4);
	tex[5]->Use(5);
	tex[6]->Use(6);
	tex[7]->Use(7);
	tex[8]->Use(8);*/
	vao->Draw();//planeShader
	glBindVertexArray(0);
}
void RenderWater(const Basic::Ptr<OpenGL::Shader> shader, 
	const Basic::Ptr<OpenGL::Texture> reflectTex, const Basic::Ptr<OpenGL::Texture> refractTex, const Basic::Ptr<OpenGL::Texture> heightTex, const Basic::Ptr<OpenGL::Texture> rippleTex,
	const Basic::Ptr<OpenGL::VAO> vao, glm::mat4 v, glm::mat4 proj) {
	shader->use();
	glm::mat4 waterModel = glm::mat4(1.0f);
	waterModel = glm::scale(waterModel, glm::vec3(30));
	shader->setMat4("model", glm::translate(waterModel, glm::vec3(0.0f, 0.0f, 0.0f)));
	shader->setMat4("view", v);
	shader->setMat4("projection", proj);
	shader->setInt("ReflectMap", 0);
	shader->setInt("HeightMap", 1);
	shader->setInt("RefractMap", 2);
	shader->setInt("RippleTex", 3);
	shader->setVec3("ViewPos", OpenGL::Camera::GetInstance()->GetPosition());
	vao->Bind();
	reflectTex->Use(0);
	heightTex->Use(1);
	refractTex->Use(2);
	rippleTex->Use(3);
	vao->Draw();//waterShader
	glBindVertexArray(0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
void RenderSkyBox(const Basic::Ptr<OpenGL::Shader> shader, const Basic::Ptr<OpenGL::Texture> tex, const Basic::Ptr<OpenGL::VAO> vao, glm::mat4 v, glm::mat4 proj) {
	shader->use();
	shader->setMat4("view", v);
	shader->setMat4("projection", proj);
	vao->Bind();
	tex->Use();
	vao->Draw();
	glBindVertexArray(0);
}