#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 target_position) {
	this->position = position;
	this->to_target_vector = target_position - position;
	this->position_target_dist = glm::length(to_target_vector);
	prev_mouse_x = -1;
	prev_mouse_y = -1;
	pitch_angle = 90.0f;
	yaw_angle = 180;
	speed = 0.5f;
	is_moving = false;
	backwards = false;
}

void Camera::change_angle(int direction_horizontal, int direction_vertical) {
	// Obliczenia wertykalne
	pitch_angle += 360.0f / WIDTH * direction_vertical;
	if (pitch_angle > 360)
		pitch_angle -= 360;
	if (pitch_angle < 0)
		pitch_angle += 360;
	//std::cout << "Pitch: " << pitch_angle << std::endl;

	float full_vector_length = glm::length(to_target_vector);
	to_target_vector.y = full_vector_length * glm::cos(glm::radians(pitch_angle));
	//std::cout << "To target vector (x: " << to_target_vector.x << " y: " << to_target_vector.y << " z: " << to_target_vector.z << ")" << std::endl;
	//std::cout << "Target vector (x: " << position.x + to_target_vector.x << " y: " << position.y + to_target_vector.y << " z: " << position.z + to_target_vector.z << ")" << std::endl;
	// Obliczenia horyzontalne;
	yaw_angle += 360.0f / WIDTH * direction_horizontal;
	if (yaw_angle > 360)
		yaw_angle -= 360;
	if (yaw_angle < 0)
		yaw_angle += 360;
	glm::vec2 horizontal_vector = glm::vec2(to_target_vector.x, to_target_vector.z);
	float hor_vector_length = full_vector_length * glm::sin(glm::radians(pitch_angle));//glm::length(horizontal_vector);
	float vector_x = hor_vector_length * glm::sin(glm::radians(yaw_angle));
	float vector_z = hor_vector_length * glm::cos(glm::radians(yaw_angle));
	to_target_vector.x = vector_x;
	to_target_vector.z = vector_z;
}

// Funkcja sprawdza czy kamera ma wlaczone poruszanie i jezeli tak to przemieszcza ja w kierunku target (zwrot zalezny od boola backwards)
// Jest wywolywana w kazdym przejsciu glownej petli
	void Camera::update() {
		if (is_moving) {
			glm::vec3 moving_dir = glm::normalize(to_target_vector) * this->speed;
			// Moving dir moze zmienic znak i wtedy sie zaczyna syf
			if (backwards) {
				moving_dir *= -1;
			}
			//std::cout << "To target vector length: " << glm::length(to_target_vector) << std::endl;
			//std::cout << "Target vector (x: " << position.x + to_target_vector.x << " y: " << position.y + to_target_vector.y << " z: " << position.z + to_target_vector.z << ")" << std::endl;
			//std::cout << "To Target vector (x: " << to_target_vector.x << " y: " <<  to_target_vector.y << " z: " << to_target_vector.z << ")" << std::endl;
			//std::cout << "Position (x: " << position.x << " y: " << position.y << " z: " << position.z << ")" << std::endl;
			//std::cout << "Moving dir (x: " << moving_dir.x << " y: " << moving_dir.y << " z: " << moving_dir.z << ")" << std::endl;
			position += moving_dir;
			//to_target_vector += moving_dir;

		}
}