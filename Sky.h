#pragma once
#include "Basic.h"
class Sky :
	public Basic
{
public:
	Sky(Model_type model_type, glm::vec3 transform, glm::vec3 scale, std::string texture_name, float rotation_angle, glm::vec3 rotation,
		bool is_visible, bool is_rotating);
	void draw();
};

