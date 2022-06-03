#include "My_Ezz_Core/Camera.hpp"
#include <glm/trigonometric.hpp>
using namespace My_Ezz;

Camera::Camera(const glm::vec3& _position,
	const glm::vec3& _rotation,
	const ProjectionMode _projectionMode)
	: m_position(_position),
	m_rotation(_rotation),
	m_projectionMode(_projectionMode)
{
	updateViewMatrix();
	updateProjectionMatrix();
}

void Camera::updateViewMatrix()
{
	//X
	float rotateInRadians_x = glm::radians(-m_rotation.x);
	glm::mat4 rotateMatrix_x(
		1, 0, 0, 0,
		0, cos(rotateInRadians_x), sin(rotateInRadians_x), 0, 
		0, -sin(rotateInRadians_x), cos(rotateInRadians_x), 0, 
		0, 0, 0, 1
	);
	//Y
	float rotateInRadians_y = glm::radians(-m_rotation.y);
	glm::mat4 rotateMatrix_y(
		cos(rotateInRadians_y), 0, -sin(rotateInRadians_y), 0, 
		0, 1, 0, 0,
		sin(rotateInRadians_y), 0, cos(rotateInRadians_y), 0, 
		0, 0, 0, 1
	);
	//Z
	float rotateInRadians_z = glm::radians(-m_rotation.z);
	glm::mat4 rotateMatrix_z(
		cos(rotateInRadians_z), sin(rotateInRadians_z), 0, 0,
		-sin(rotateInRadians_z), cos(rotateInRadians_z), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);

	glm::mat4 translateMatrix(
		1,			  0,			  0,			 0,
		0,			  1,			  0,			 0,
		0,			  0,			  1,			 0,
		-m_position[0], -m_position[1], -m_position[2], 1
	);
	m_viewMatrix = rotateMatrix_y * rotateMatrix_x * translateMatrix;
}

void Camera::updateProjectionMatrix()
{
	if (m_projectionMode == ProjectionMode::Perspective)
	{
		float r = 0.1f;
		float t = 0.1f;
		float f = 10.0f;
		float n = 0.1f;
		m_projectionMatrix = glm::mat4(n / r, 0, 0, 0,
									   0, n/t, 0, 0,
									   0, 0, (-f - n) / (f - n), -1,
									   0, 0, -2*f*n/(f-n), 0);
	}
	else
	{
		float r = 2.0f;
		float t = 2.0f;
		float f = 100.0f;
		float n = 0.1f;
		m_projectionMatrix = glm::mat4(1 / r, 0, 0, 0,
									   0, 1 / t, 0, 0,
									   0, 0, -2/(f-n), 0,
									   0, 0, (-f-n) / (f - n), 1);
	}
}

void Camera::setPosition(const glm::vec3& _position)
{
	m_position = _position;
	updateViewMatrix();
}

void Camera::setRotation(const glm::vec3& _rotation)
{
	m_rotation = _rotation;
	updateViewMatrix();
}

void Camera::setPositionAndRotation(const glm::vec3& _position, const glm::vec3& _rotation)
{
	m_position = _position;
	m_rotation = _rotation;
	updateViewMatrix();
}

void Camera::setProjectionMode(const ProjectionMode _projectionMode)
{
	m_projectionMode = _projectionMode;
	updateProjectionMatrix();
}