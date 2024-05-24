#include "emtpch.h"
#include "Camera.h"

namespace EMT {
	void Camera::processKeyBoard(Camera_Movement movement, float deltaTime) {
		float velocity = MoveSpeed * deltaTime;
		if (movement == Camera_Movement::FORWARD)
			Position += Front * velocity;
		if (movement == Camera_Movement::BACK)
			Position -= Front * velocity;
		if (movement == Camera_Movement::LEFT)
			Position -= Right * velocity;
		if (movement == Camera_Movement::RIGHT)
			Position += Right * velocity;
	}

	void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;
		Yam += xoffset;
		Pitch += yoffset;
		if (constrainPitch) {
			if (Pitch > 89.f)
				Pitch = 89.f;
			if (Pitch < -89.f)
				Pitch = -89.f;
		}
		updateCameraVectors();
	}

	void Camera::processMouseScroll(float yoffset) {
		if (Zoom < 1.f)
			Zoom = 1.f;
		else if (Zoom > 45.f)
			Zoom = 45.f;
		else Zoom -= yoffset;
	}

	void Camera::updateCameraVectors() {
		vec3 front(1.f);
		front.x = glm::cos(glm::radians(Pitch)) * glm::cos(glm::radians(Yam));
		front.y = glm::sin(glm::radians(Pitch));
		front.z = glm::cos(glm::radians(Pitch)) * glm::sin(glm::radians(Yam));
		Front = glm::normalize(front);
		Right = glm::normalize(glm::cross(Front, WordUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
}