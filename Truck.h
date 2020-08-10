#pragma once
#include "Basic.h"
class Truck :
	public Basic
{
private:
	//glm::vec3 car_location;
	//float car_heading_angle;
	float car_speed;
	float steer_angle;
	float wheel_base;
	float friction;
	bool can_move;
	bool accelerating_forwards;
	bool accelerating_backwards;
public:
	float acceleration;
public:
	Truck(Model_type model_type, glm::vec3 transform, glm::vec3 scale, std::string texture_name, float rotation_angle, glm::vec3 rotation, bool is_visible, bool is_rotating);
	void movement();
	bool collision(Basic* other);
	void change_angle(float value);
};

