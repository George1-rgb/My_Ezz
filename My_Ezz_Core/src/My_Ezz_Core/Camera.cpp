#include "My_Ezz_Core/Camera.hpp"
#include <glm/trigonometric.hpp>
#include <glm/ext/matrix_transform.hpp>

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

glm::mat4 Camera::getViewMatrix()
{
	if (m_bUpdateViewMatrix)
		updateViewMatrix();
	m_bUpdateViewMatrix = false;
	return m_viewMatrix;
}

void Camera::updateViewMatrix()
{
	const float fRollInRadians = glm::radians(m_rotation.x);
	const float fPitchInRadians = glm::radians(m_rotation.y);
	const float fYawInRadians = glm::radians(m_rotation.z);




	//X
	const glm::mat3 rotateMatrix_x(
		1, 0, 0,
		0, cos(fRollInRadians), sin(fRollInRadians),
		0, -sin(fRollInRadians), cos(fRollInRadians)
	);
	//Y
	const glm::mat3 rotateMatrix_y(
		cos(fPitchInRadians), 0, -sin(fPitchInRadians),
		0, 1, 0,
		sin(fPitchInRadians), 0, cos(fPitchInRadians)
	);
	//Z
	const glm::mat3 rotateMatrix_z(
		cos(fYawInRadians), sin(fYawInRadians), 0,
		-sin(fYawInRadians), cos(fYawInRadians), 0,
		0, 0, 1
	);

	const glm::mat3 eulerRotateMatrix = rotateMatrix_z * rotateMatrix_y * rotateMatrix_x;

	m_direction = glm::normalize(worldForward * eulerRotateMatrix);
	m_right = glm::normalize(worldRight * eulerRotateMatrix);
	m_up = glm::cross(m_right, m_direction);

	m_viewMatrix = glm::lookAt(m_position, m_position+m_direction, m_up);
}

void Camera::updateProjectionMatrix()
{
	if (m_projectionMode == ProjectionMode::Perspective)
	{
		float r = 0.1f;
		float t = 0.1f;
		float f = 100.0f;
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
	m_bUpdateViewMatrix = true;
}

void Camera::setRotation(const glm::vec3& _rotation)
{
	m_rotation = _rotation;
	m_bUpdateViewMatrix = true;
}

void Camera::setPositionAndRotation(const glm::vec3& _position, const glm::vec3& _rotation)
{
	m_position = _position;
	m_rotation = _rotation;
	m_bUpdateViewMatrix = true;
}

void Camera::setProjectionMode(const ProjectionMode _projectionMode)
{
	m_projectionMode = _projectionMode;
	updateProjectionMatrix();
}

void Camera::moveForward(const float fDelta)
{
	m_position += m_direction * fDelta;
	m_bUpdateViewMatrix = true;
}
void Camera::moveRight(const float fDelta)
{
	m_position += m_right * fDelta;
	m_bUpdateViewMatrix = true;
}

void Camera::moveUp(const float fDelta)
{
	m_position += worldUp * fDelta;
	m_bUpdateViewMatrix = true;
}

void Camera::add_movement_and_rotation(const glm::vec3& movement_delta, const glm::vec3 rotation_delta)
{
	m_position += m_direction * movement_delta.x;
	m_position += m_right * movement_delta.y;
	m_position += m_up * movement_delta.z;

	m_rotation += rotation_delta;
	m_bUpdateViewMatrix = true;
}
