#include "Truck.h"

Truck::Truck(Model_type model_type, glm::vec3 transform, glm::vec3 scale, std::string texture_name, float rotation_angle, glm::vec3 rotation, bool is_visible, bool is_rotating)
	:Basic(model_type, transform, scale, texture_name, rotation_angle, rotation, is_visible, is_rotating) {
	car_speed = 0.0f;
	steer_angle = 0.0f;
	can_move = true;
	wheel_base = 0.0f;
	acceleration = 0.0f;
	friction = 0.2f;
	accelerating_backwards = false;
	accelerating_forwards = false;
}

// Trzeba to zrobic inaczej!
void Truck::movement() {
	// Obliczenia zwiazane z przyspieszeniem i predkoscia
	if (acceleration > 0.0f && accelerating_backwards == false) {
		accelerating_forwards = true;
	}
	if (acceleration < 0.0f && accelerating_forwards == false) {
		accelerating_backwards = true;
	}
	if (accelerating_forwards)
	{
		this->car_speed += acceleration;
		this->car_speed -= friction;
		if (this->car_speed > 20.0f) {
			this->car_speed = 20.0f;
		}
	}
	if (accelerating_backwards)
	{
		this->car_speed += -acceleration;
		this->car_speed -= friction;
		if (this->car_speed > 20.0f) {
			this->car_speed = 20.0f;
		}
	}
	if (accelerating_forwards) {
		if (this->car_speed <= 0.0f) {
			accelerating_forwards = false;
			this->car_speed = 0.0f;
		}
	}
	if (accelerating_backwards) {
		if (this->car_speed <= 0.0f) {
			accelerating_backwards = false;
			this->car_speed = 0.0f;
		}
	}
	// Pozycje przednia i tylna w przestrzenii modelu
	glm::vec4 temp1 = getMMatrix() * glm::vec4(0.0f, 0.0f, -50.0f, 1.0f);
	glm::vec4 temp2 = getMMatrix() * glm::vec4(0.0f, 0.0f, 50.0f, 1.0f);
	glm::vec3 punkt_przedni = glm::vec3(temp1.x, temp1.y, temp1.z);
	glm::vec3 punkt_tylny = glm::vec3(temp2.x, temp2.y, temp2.z);

	float speed_x;
	float speed_z;
	float speed_x_steer;
	float speed_z_steer;
	// Obliczamy wektory predkosci
	if (accelerating_backwards) {
		speed_x = glm::sin(glm::radians(this->rotation_angle)) * -this->car_speed * float(glfwGetTime());
		speed_z = glm::cos(glm::radians(this->rotation_angle)) * -this->car_speed * float(glfwGetTime());
		speed_x_steer = glm::sin(glm::radians(this->rotation_angle + this->steer_angle)) * -this->car_speed * float(glfwGetTime());
		speed_z_steer = glm::cos(glm::radians(this->rotation_angle + this->steer_angle)) * -this->car_speed * float(glfwGetTime());
	}
	else {
		speed_x = glm::sin(glm::radians(this->rotation_angle)) * this->car_speed * float(glfwGetTime());
		speed_z = glm::cos(glm::radians(this->rotation_angle)) * this->car_speed * float(glfwGetTime());
		speed_x_steer = glm::sin(glm::radians(this->rotation_angle + this->steer_angle)) * this->car_speed * float(glfwGetTime());
		speed_z_steer = glm::cos(glm::radians(this->rotation_angle + this->steer_angle)) * this->car_speed * float(glfwGetTime());
	}
	// Obliczenie nowych pozycji punktow
	glm::vec3 punkt_przedni_new = punkt_przedni + glm::vec3(speed_x_steer, 0.0f, speed_z_steer);
	glm::vec3 punkt_tylny_new = punkt_tylny + glm::vec3(speed_x, 0.0f, speed_z);

	// Obliczamy nowy punkt srodkowy (wspolrzedna modelu)
	glm::vec3 new_pos = glm::vec3(punkt_przedni_new.x + punkt_tylny_new.x, punkt_przedni_new.y + punkt_tylny_new.y, punkt_przedni_new.z + punkt_tylny_new.z) / 2.0f;
	this->translate(new_pos);
	
	// Obliczamy nowy kat
	this->rotation_angle = 180.0f*(glm::atan(punkt_tylny_new.x - punkt_przedni_new.x, punkt_tylny_new.z - punkt_przedni_new.z))/PI;
	//std::cout << "Position: x: " << new_pos.x << " y: " << new_pos.y << " z: " << new_pos.z << std::endl;
	//std::cout << "Rotation angle: " << this->rotation_angle << std::endl;
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

	wheel_base = maxTruckVec.x - minTruckVec.x;
	//std::cout << "X base:" << maxTruckVec.x - minTruckVec.x << std::endl;
	//std::cout << "Z base:" << maxTruckVec.z - minTruckVec.z << std::endl;

	if ((maxTruckVec.x < maxOtherVec.x && maxTruckVec.x > minOtherVec.x&& maxTruckVec.z < maxOtherVec.z && maxTruckVec.z > minOtherVec.z) ||
		(minTruckVec.x < maxOtherVec.x && minTruckVec.x > minOtherVec.x && minTruckVec.z < maxOtherVec.z && minTruckVec.z > minOtherVec.z)) {
		std::cout << "Collision!" << std::endl;
		return true;
	}
	return false;
}

void Truck::change_angle(float value) {
	this->steer_angle += value;
	//std::cout << "Steer angle: " << steer_angle << std::endl;
	if (this->steer_angle > 90.0f)
		this->steer_angle = 90.0f;
	if (this->steer_angle < -90.0f)
		this->steer_angle = -90.0f;
}
