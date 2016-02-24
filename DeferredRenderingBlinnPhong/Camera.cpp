#include "Camera.h"



Camera::Camera()
{
	position = glm::vec3(0.0f, 0.0f, 3.0f);
	forward = glm::vec3(0.0f, 0.0f, -1.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	speed = 10.0f;
}

glm::vec3 Camera::getForward() {
	return forward;
}

glm::vec3 Camera::getPos() {
	return position;
}

glm::vec3 Camera::getUp() {
	return up;
}

void Camera::setSpeed(GLfloat newSpeed) {
	speed = newSpeed;
}

void Camera::moveF() {
	position += speed * forward;
}

void Camera::moveB() {
	position -= speed * forward;
}

void Camera::moveL() {
	position -= glm::normalize(glm::cross(forward, up)) * speed; // Normalisation prevents different movement speeds based of camera orientations
}

void Camera::moveR() {
	position += glm::normalize(glm::cross(forward, up)) * speed;
}

void Camera::update(bool active[], GLfloat pitch, GLfloat yaw) {
	if (active[GLFW_KEY_W]) moveF();
	if (active[GLFW_KEY_S]) moveB();
	if (active[GLFW_KEY_A]) moveL();
	if (active[GLFW_KEY_D]) moveR();
	forward.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	forward.y = sin(glm::radians(pitch));
	forward.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	forward = glm::normalize(forward);
}