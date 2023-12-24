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
		void SetFarClipPlane(const float far);
		void SetNearClipPlane(const float near);
		void SetViewportSize(const float width, const float height);
		void SetFieldOfView(const float fieldOfView);

		const glm::mat4 getViewMatrix();
		const glm::mat4 getProjectionMatrix() const { return m_projectionMatrix; }
		const float GetFarClipPlane() const { return m_FarClipPlane; }
		const float GetNearClipPlane() const { return m_NearClipPlane; }
		const float GetFieldOfView() const { return m_FieldOfView; }

		void moveForward(const float fDelta);
		void moveRight(const float fDelta);
		void moveUp(const float fDelta);

		const glm::vec3& getPosition() const { return m_position; }
		const glm::vec3& getRotation() const { return m_rotation; }

		//movement.x - forward, movement.y - right, movement.z - up
		//rotation.x - roll, rotation.y - pitch, rotation.z - yaw
		void add_movement_and_rotation(const glm::vec3& movement_delta, const glm::vec3 rotation_delta);

	private:
		void updateViewMatrix();
		void updateProjectionMatrix();

		glm::vec3 m_position;
		glm::vec3 m_rotation; // X - Roll, Y - Pitch, Z - Yaw
		ProjectionMode m_projectionMode;

		glm::vec3 m_direction;
		glm::vec3 m_right;
		glm::vec3 m_up;

		float m_FarClipPlane = 1000.f;
		float m_NearClipPlane = 0.1f;
		float m_ViewportWidth = 800.f;
		float m_ViewportHeight = 600.f;
		float m_FieldOfView = 60.f;

		static constexpr glm::vec3 worldUp{ 0.0f, 0.0f, 1.0f };
		static constexpr glm::vec3 worldRight{ 0.0f, -1.0f, 0.0f };
		static constexpr glm::vec3 worldForward{ 1.0f, 0.0f, 0.0f };

		glm::mat4 m_viewMatrix;
		glm::mat4 m_projectionMatrix;
		bool m_bUpdateViewMatrix = false;
	};
}