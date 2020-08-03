#include "Arrow.h"

Arrow::Arrow(Model_type model_type, glm::vec3 transform, glm::vec3 scale, std::string texture_name, float rotation_angle, glm::vec3 rotation, bool is_visible, bool is_rotating)
:Basic(model_type, transform, scale, texture_name, rotation_angle, rotation, is_visible, is_rotating) {
	levitation_pos = 0.0f;
	levitation_speed = 0.01f;
	levitation_limit = 0.1f;
}

void Arrow::draw() {
	//std::cout << "Levitation pos: " << levitation_pos << std::endl;
	if (levitation_pos + levitation_speed >= levitation_limit || levitation_pos + levitation_speed <= -levitation_limit) {
		levitation_speed = -levitation_speed;
	}
	levitation_pos += levitation_speed;
	Arrow::translate(transform + glm::vec3(0.0f, levitation_pos, 0.0f));
	Basic::draw();
}
