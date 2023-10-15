#include "../include/camera.h"

Camera::Camera(glm::vec3 position, float yaw, float pitch)
{
	this->position = position;
	this->yaw = yaw;
	this->pitch = pitch;
	updateVectors();
}

glm::mat4x4 Camera::viewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

void Camera::moveForwards(float distance)
{
	position += distance * front;
}

void Camera::moveRight(float distance)
{
	position += distance * right;
}

void Camera::moveVertical(float distance)
{
	position.y += distance;
}

void Camera::adjustOrientation(float deltaYaw, float deltaPitch)
{
	yaw += deltaYaw;
	pitch += deltaPitch;

	if(pitch <= -90.0f)
		pitch = -89.9f;

	if(pitch >= 90.0f)
		pitch = 89.9f;

	while(yaw > 180.0f)
		yaw -= 360.0f;

	while(yaw < -180.0f)
		yaw += 360.0f;

	updateVectors();
}

void Camera::updateVectors()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
	up = glm::normalize(glm::cross(right, front));
}
