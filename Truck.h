#pragma once
#include "Basic.h"
class Truck :
	public Basic
{
private:
	float velocity;
	float max_velocity;
	float wheels_angle;
	bool going_backwards;
	bool can_accelerate_forward;
	bool can_accelerate_backwards;
public:
	Truck(Model_type model_type, glm::vec3 transform, glm::vec3 scale, std::string texture_name, float rotation_angle, glm::vec3 rotation, bool is_visible, bool is_rotating);
	void movement(bool dir[4]);
	bool collision(Basic* other);
};

