#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "constants.h"

class Camera
{
public:
	glm::vec3 position;
	glm::vec3 to_target_vector;
	double prev_mouse_x;
	double prev_mouse_y;
	float position_target_dist;
	float yaw_angle;
	float pitch_angle;
	bool is_moving;
	bool backwards;
	float speed;

	Camera(glm::vec3 position, glm::vec3 target_position);
	void change_angle(int direction_horizontal, int direction_vertical);
	void update(bool arrow_up, bool arrow_down, bool arrow_left, bool arrow_right);
};
