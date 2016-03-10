#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glew.h>
#include <glfw3.h>

class Camera
{
public:
	Camera();
	glm::vec3 getPos();
	glm::vec3 getForward();
	glm::vec3 getUp();
	void setSpeed(GLfloat newSpeed);
	void moveF();
	void moveB();
	void moveR();
	void moveL();
	void update(bool active[], GLfloat pitch, GLfloat yaw);
private: 
	glm::vec3 position, forward, up, right;
	GLfloat speed;
};

#endif 
