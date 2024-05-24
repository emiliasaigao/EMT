#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace EMT {
	enum Camera_Movement {
		FORWARD,
		BACK,
		LEFT,
		RIGHT
	};

	// 默认参数
	const float YAM = -90.f; // 仰角
	const float PITCH = 0.f; // 水平角
	const float SPEED = 4.f;
	const float SENSITIVITY = 0.1f;
	const float ZOOM = 45.f;

	class Camera {
	public:
		typedef	typename glm::vec3		vec3;
		typedef typename glm::mat4		mat4;

		vec3 Position;
		vec3 Front;
		vec3 Up;
		vec3 Right;
		vec3 WordUp;

		float Yam;
		float Pitch;

		float MoveSpeed;
		float MouseSensitivity;
		float Zoom;

		Camera(vec3 position = vec3(0.f, 0.f, 0.f), vec3 up = vec3(0.f, 1.f, 0.f),
			float yam = YAM, float pitch = PITCH)
			: Front(vec3(0.f, 0.f, -1.f)), MoveSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
			Position = position;
			WordUp = up;
			Yam = yam;
			Pitch = pitch;
			updateCameraVectors();
		}

		Camera(float positionx, float positiony, float positionz,
			float upx, float upy, float upz, float yam, float pitch)
			:Front(vec3(0.f, 0.f, -1.f)), MoveSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
			Position = vec3(positionx, positiony, positionz);
			WordUp = vec3(upx, upy, upz);
			Yam = yam;
			Pitch = pitch;
			updateCameraVectors();
		}

		mat4 getViewMatrix() const { return glm::lookAt(Position, Position + Front, Up); }
		mat4 getProjectionMatrix() const {
			return glm::perspective(glm::radians(Zoom), float(16.f / 9.f), 0.1f, 100.f);
		}
		void processKeyBoard(Camera_Movement movement, float deltaTime);
		void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
		void processMouseScroll(float yoffset);

	private:
		void updateCameraVectors();
	};
}
