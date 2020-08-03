#include "Truck.h"

Truck::Truck(Model_type model_type, glm::vec3 transform, glm::vec3 scale, std::string texture_name, float rotation_angle, glm::vec3 rotation, bool is_visible, bool is_rotating)
	:Basic(model_type, transform, scale, texture_name, rotation_angle, rotation, is_visible, is_rotating) {
	velocity = 0.0f;
	max_velocity = 5.0f;
	wheels_angle = 0.0f;
	going_backwards = false;
	can_accelerate_forward = true;
	can_accelerate_backwards = true;
}

// Trzeba to zrobic inaczej!
void Truck::movement(bool dir[4]) {
	// Spowalnianie bez wciskania gazu
	if (going_backwards) {
		velocity += 0.01f;
		if (velocity >= 0) {
			velocity = 0.0f;
			can_accelerate_forward = true;
		}
	}
	else {
		velocity -= 0.01f;
		if (velocity <= 0.0f) {
			velocity = 0.0f;
			can_accelerate_backwards = true;

		}
	}
	// Zmiana kata

	// Gaz wcisniety
	if (dir[0] && can_accelerate_forward) {
		this->going_backwards = false;
		velocity += 0.02f;
	}
	// Wsteczny
	else if (dir[2] && can_accelerate_backwards) {
		this->going_backwards = true;
		velocity -= 0.02f;
	}
	this->transform.x += velocity;
}

bool Truck::collision(Basic* other) {
	glm::mat4 otherM = other->getMMatrix();
	glm::mat4 truckM = this->getMMatrix();
	glm::vec4 minTruckVec = truckM * glm::vec4(this->min_vert_x, 0.0f, this->min_vert_z, 1.0f);
	//std::cout << "Min truck x:" << minTruckVec.x << " z:" << minTruckVec.z  << std::endl;
	glm::vec4 maxTruckVec = truckM * glm::vec4(this->max_vert_x, 0.0f, this->max_vert_z, 1.0f);
	//std::cout << "Max truck x:" << maxTruckVec.x << " z:" << maxTruckVec.z << std::endl;
	glm::vec4 minOtherVec = otherM * glm::vec4(other->min_vert_x, 0.0f, other->min_vert_z, 1.0f);
	//std::cout << "Min other x:" << minOtherVec.x << " z:" << minOtherVec.z << std::endl;
	glm::vec4 maxOtherVec = otherM * glm::vec4(other->max_vert_x, 0.0f, other->max_vert_z, 1.0f);
	//std::cout << "Max other x:" << maxOtherVec.x << " z:" << maxOtherVec.z << std::endl;

	if (minTruckVec.x > maxTruckVec.x) {
		std::swap(minTruckVec.x, maxTruckVec.x);
	}
	if (minTruckVec.z > maxTruckVec.z) {
		std::swap(minTruckVec.z, maxTruckVec.z);
	}
	if (minOtherVec.x > maxOtherVec.x) {
		std::swap(minOtherVec.x, maxOtherVec.x);
	}
	if (minOtherVec.z > maxOtherVec.z) {
		std::swap(minOtherVec.z, maxOtherVec.z);
	}

	if ((maxTruckVec.x < maxOtherVec.x && maxTruckVec.x > minOtherVec.x&& maxTruckVec.z < maxOtherVec.z && maxTruckVec.z > minOtherVec.z) ||
		(minTruckVec.x < maxOtherVec.x && minTruckVec.x > minOtherVec.x && minTruckVec.z < maxOtherVec.z && minTruckVec.z > minOtherVec.z)) {
		std::cout << "Collision!" << std::endl;
		return true;
	}
	return false;
}
