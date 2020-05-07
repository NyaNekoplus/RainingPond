#include "Camera.h"
#include "util.h"

using namespace OpenGL;

bool Camera::firstMouse = true;
const float Camera::YAW = -90.0f;
const float Camera::PITCH = 0.0f;
const float Camera::SPEED = 10.0f;
const float Camera::SENSITIVITY = 0.1f;
const float Camera::FOV = 45.0f;

Camera::Camera(vec3 CamPosition,vec3 CamFront,vec3 up,
	float yaw, float pitch, float sensitivity, float speed, float fov):
	Position(CamPosition),Front(CamFront),WorldUp(up),
	Yaw(yaw),Pitch(pitch), MovementSpeed(speed),MouseSensitivity(sensitivity),Fov(fov)
{
	updateCameraVector();
}
Camera* Camera::instance = new Camera();
Camera* Camera::GetInstance() { return instance; }

void Camera::updateCameraVector() {
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
}

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(Position, Position + Front, Up);
}
glm::mat4 Camera::GetProjectionMatrix(PROJ_MODE mode) {
	switch (mode)
	{
	case OpenGL::Camera::PRESPECTIVE:
		return glm::perspective(glm::radians(Fov), Util::SCR_WIDTH / Util::SCR_HEIGHT, 0.1f, 100.0f);
		break;
	case OpenGL::Camera::ORTHONORMAL:
		return glm::ortho(0.0f, Util::SCR_WIDTH, 0.0f, Util::SCR_HEIGHT, 0.1f, 100.0f);
		break;
	default:
		printf("Projection mode unset.\n");
		break;
	}
	
}
glm::vec3 Camera::GetPosition() {
	return this->Position;
}

void Camera::ProcessKeyboard(int key) {
	float velocity = MovementSpeed * deltaTime;
	if (key == 87)
		Position += Front * velocity;
//	printf("%f", Position.x);
	if (key == 83)
		Position -= Front * velocity;
	if (key == 65)
		Position -= Right * velocity;
	if (key == 68)
		Position += Right * velocity;
}
void Camera::ProcessMouseMovement(double xOffset, double yOffset) {
	xOffset *= MouseSensitivity;
	yOffset *= MouseSensitivity;

	Yaw += xOffset;
	Pitch += yOffset;

	if (Pitch > 89.0f)
		Pitch = 89.0f;
	if (Pitch < -89.0f)
		Pitch = -89.0f;
	updateCameraVector();
}
void Camera::ProcessMouseScroll(float yOffset) {
	if (Fov >= 1.0f && Fov <= 45.0f)
		Fov -= yOffset;
	if (Fov <= 1.0f)
		Fov = 1.0f;
	if (Fov >= 45.0f)
		Fov = 45.0f;
}
float Camera::GetFrameDelay() { return deltaTime; }
void Camera::CalcFrameDelay(float time) {
	float currentFrame = time;
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}