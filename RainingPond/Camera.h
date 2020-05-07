#ifndef CAMERA_H_
#define CAMERA_H_

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


using namespace glm;

namespace OpenGL {
	class Camera {
	public:
		enum PROJ_MODE
		{
			PRESPECTIVE,
			ORTHONORMAL
		};
		static Camera* GetInstance();
		void ProcessKeyboard(int key);
		void ProcessMouseMovement(double xOffset, double yOffset);
		void ProcessMouseScroll(float yOffset);
		void CalcFrameDelay(float time);
		float GetFrameDelay();
		mat4 GetViewMatrix();
		mat4 GetProjectionMatrix(PROJ_MODE mode = PROJ_MODE::PRESPECTIVE);
		vec3 GetPosition();
		
		static bool firstMouse;
	private:
		static Camera* instance;
		Camera(vec3 CamPosition = vec3(0.0f, 10.0f, 28.0f),
			vec3 CamFront = vec3(0.0f, 0.0f, 0.0f),
			vec3 up = vec3(0.0f, 1.0f, 0.0f),
			float yaw = YAW, float pitch = PITCH,float sensitivity= SENSITIVITY,float speed=SPEED,float fov=FOV);
		void updateCameraVector();
		vec3 Position;
		vec3 Front;
		vec3 Right;
		vec3 Up;
		vec3 WorldUp;
		float Yaw;
		float Pitch;
		float MovementSpeed;
		float MouseSensitivity;
		float Fov;

		float deltaTime;
		float lastFrame;

		static const float YAW;
		static const float PITCH;
		static const float SPEED;
		static const float SENSITIVITY;
		static const float FOV;

		
	};
}

#endif // !CAMERA_H_
