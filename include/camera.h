#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera(glm::vec3 potision, float yaw, float pitch);
	glm::mat4x4 viewMatrix();
	void moveForwards(float distance);
	void moveRight(float distance);
	void moveVertical(float distance);
	void adjustOrientation(float deltaYaw, float deltaPitch);
private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;
	float yaw;
	float pitch;
	void updateVectors();
};

#endif // CAMERA_H
