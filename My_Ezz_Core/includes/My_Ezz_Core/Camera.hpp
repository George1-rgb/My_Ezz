#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/vec3.hpp>

namespace My_Ezz
{
	class Camera
	{
	public:
		enum class ProjectionMode
		{
			Perspective,
			Orthographic
		};

		Camera(const glm::vec3& _position = { 0.0f, 0.0f, 0.0f },
			   const glm::vec3& _rotation = { 0.0f, 0.0f, 0.0f },
			   const ProjectionMode _projectionMode = ProjectionMode::Perspective);
		void setPosition(const glm::vec3& _position);
		void setRotation(const glm::vec3& _rotation);
		void setPositionAndRotation(const glm::vec3& _position, const glm::vec3& _rotation);
		void setProjectionMode(const ProjectionMode _projectionMode);
		glm::mat4 getViewMatrix() const { return m_viewMatrix; }
		glm::mat4 getProjectionMatrix() const { return m_projectionMatrix; }
	private:
		void updateViewMatrix();
		void updateProjectionMatrix();

		glm::vec3 m_position;
		glm::vec3 m_rotation;
		ProjectionMode m_projectionMode;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_projectionMatrix;
	};
}