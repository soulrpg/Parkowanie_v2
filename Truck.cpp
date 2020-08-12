#include "Truck.h"

struct _vec2 { float x; float y; };
typedef struct _vec2 vec2;

int start(vec2* vertices1, vec2* vertices2);

Truck::Truck(Model_type model_type, glm::vec3 transform, glm::vec3 scale, std::string texture_name, float rotation_angle, glm::vec3 rotation,
	bool is_visible, bool is_rotating, glm::vec3 transform_wheel_1, glm::vec3 transform_wheel_2)
	:Basic(model_type, transform, scale, texture_name, rotation_angle, rotation, is_visible, is_rotating) {
	this->transform_wheel_1 = transform_wheel_1;
	this->transform_wheel_2 = transform_wheel_2;
	car_speed = 0.0f;
	steer_angle = 0.0f;
	can_move = true;
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

void Truck::draw() {
	if (this->is_visible)
	{
		Basic::sp->use();
		glm::mat4 M = glm::mat4(1.0f);
		M = glm::translate(M, transform);
		M = glm::rotate(M, glm::radians(rotation_angle), rotation);
		glm::mat4 M2 = M;
		M = glm::scale(M, scale);
		glUniformMatrix4fv(Basic::sp->u("M"), 1, false, glm::value_ptr(M));
		glEnableVertexAttribArray(Basic::sp->a("vertex"));
		glVertexAttribPointer(Basic::sp->a("vertex"), 4, GL_FLOAT, false, 0, Basic::vertices[model_type]);

		glEnableVertexAttribArray(Basic::sp->a("normal"));
		glVertexAttribPointer(Basic::sp->a("normal"), 4, GL_FLOAT, false, 0, Basic::normals[model_type]);

		glEnableVertexAttribArray(Basic::sp->a("texCoord"));
		glVertexAttribPointer(Basic::sp->a("texCoord"), 2, GL_FLOAT, false, 0, Basic::texCoords[model_type]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_handle);
		glUniform1i(sp->u("tex"), 0);
		glDrawArrays(GL_TRIANGLES, 0, Basic::vertexCount[model_type]);
		glDisableVertexAttribArray(Basic::sp->a("vertex"));
		glDisableVertexAttribArray(Basic::sp->a("normal"));
		glDisableVertexAttribArray(Basic::sp->a("texCoord"));

		// WHEEL 1
		M = glm::translate(M2, transform_wheel_1);
		M = glm::rotate(M, glm::radians(-steer_angle + 180.f), rotation);
		M = glm::scale(M, scale);
		glUniformMatrix4fv(Basic::sp->u("M"), 1, false, glm::value_ptr(M));
		glEnableVertexAttribArray(Basic::sp->a("vertex"));
		glVertexAttribPointer(Basic::sp->a("vertex"), 4, GL_FLOAT, false, 0, Basic::vertices[wheel]);

		glEnableVertexAttribArray(Basic::sp->a("normal"));
		glVertexAttribPointer(Basic::sp->a("normal"), 4, GL_FLOAT, false, 0, Basic::normals[wheel]);

		glEnableVertexAttribArray(Basic::sp->a("texCoord"));
		glVertexAttribPointer(Basic::sp->a("texCoord"), 2, GL_FLOAT, false, 0, Basic::texCoords[wheel]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_handle);
		glUniform1i(sp->u("tex"), 0);
		glDrawArrays(GL_TRIANGLES, 0, Basic::vertexCount[wheel]);
		glDisableVertexAttribArray(Basic::sp->a("vertex"));
		glDisableVertexAttribArray(Basic::sp->a("normal"));
		glDisableVertexAttribArray(Basic::sp->a("texCoord"));

		// WHEEL 2
		M = glm::translate(M2, transform_wheel_2);
		M = glm::rotate(M, glm::radians(-steer_angle), rotation);
		M = glm::scale(M, scale);
		glUniformMatrix4fv(Basic::sp->u("M"), 1, false, glm::value_ptr(M));
		glEnableVertexAttribArray(Basic::sp->a("vertex"));
		glVertexAttribPointer(Basic::sp->a("vertex"), 4, GL_FLOAT, false, 0, Basic::vertices[wheel]);

		glEnableVertexAttribArray(Basic::sp->a("normal"));
		glVertexAttribPointer(Basic::sp->a("normal"), 4, GL_FLOAT, false, 0, Basic::normals[wheel]);

		glEnableVertexAttribArray(Basic::sp->a("texCoord"));
		glVertexAttribPointer(Basic::sp->a("texCoord"), 2, GL_FLOAT, false, 0, Basic::texCoords[wheel]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_handle);
		glUniform1i(sp->u("tex"), 0);
		glDrawArrays(GL_TRIANGLES, 0, Basic::vertexCount[wheel]);
		glDisableVertexAttribArray(Basic::sp->a("vertex"));
		glDisableVertexAttribArray(Basic::sp->a("normal"));
		glDisableVertexAttribArray(Basic::sp->a("texCoord"));
	}
}

// Pomocnicze funkcje do sprawdzania kolizji
bool same_side(glm::vec3 p1, glm::vec3 p2, glm::vec3 a, glm::vec3 b){
	glm::vec3 cp1 = glm::cross(b - a, p1 - a);
	glm::vec3 cp2 = glm::cross(b - a, p2 - a);
	return (glm::dot(cp1, cp2) >= 0);
}

bool point_in_triangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c) {
	return (same_side(p, a, b, c) && same_side(p, b, a, c) && same_side(p, c, a, b));
}

bool Truck::collision(Basic* other) {
	glm::mat4 t_matrix = this->getMMatrix();
	glm::vec4 point_1 = t_matrix * glm::vec4(this->max_vert_x, 5.0f, this->max_vert_z, 1.0f);
	glm::vec4 point_2 = t_matrix * glm::vec4(this->min_vert_x, 5.0f, this->max_vert_z, 1.0f);
	glm::vec4 point_3 = t_matrix * glm::vec4(this->min_vert_x, 5.0f, this->min_vert_z, 1.0f);
	glm::vec4 point_4 = t_matrix * glm::vec4(this->max_vert_x, 5.0f, this->min_vert_z, 1.0f);

	// Punkty ciezarowki
	glm::vec3 t_point_1 = glm::vec3(point_1.x, point_1.y, point_1.z);
	glm::vec3 t_point_2 = glm::vec3(point_2.x, point_2.y, point_2.z);
	glm::vec3 t_point_3 = glm::vec3(point_3.x, point_3.y, point_3.z);
	glm::vec3 t_point_4 = glm::vec3(point_4.x, point_4.y, point_4.z);

	t_matrix = other->getMMatrix();
	point_1 = t_matrix * glm::vec4(other->max_vert_x, 5.0f, other->max_vert_z, 1.0f);
	point_2 = t_matrix * glm::vec4(other->min_vert_x, 5.0f, other->max_vert_z, 1.0f);
	point_3 = t_matrix * glm::vec4(other->min_vert_x, 5.0f, other->min_vert_z, 1.0f);
	point_4 = t_matrix * glm::vec4(other->max_vert_x, 5.0f, other->min_vert_z, 1.0f);
	
	// Punkty innego obiektu
	glm::vec3 o_point_1 = glm::vec3(point_1.x, point_1.y, point_1.z);
	glm::vec3 o_point_2 = glm::vec3(point_2.x, point_2.y, point_2.z);
	glm::vec3 o_point_3 = glm::vec3(point_3.x, point_3.y, point_3.z);
	glm::vec3 o_point_4 = glm::vec3(point_4.x, point_4.y, point_4.z);

	// Jezeli jakikolwiek punkt ciezarowki znajduje sie w jednym z dwoch trojkatow stworzonych z punktow obiektu, wtedy doszlo do kolizji
	if (point_in_triangle(t_point_1, o_point_1, o_point_2, o_point_3) ||
		point_in_triangle(t_point_1, o_point_1, o_point_2, o_point_4) ||
		point_in_triangle(t_point_2, o_point_1, o_point_2, o_point_3) ||
		point_in_triangle(t_point_2, o_point_1, o_point_2, o_point_4) ||
		point_in_triangle(t_point_3, o_point_1, o_point_2, o_point_3) ||
		point_in_triangle(t_point_3, o_point_1, o_point_2, o_point_4) ||
		point_in_triangle(t_point_4, o_point_1, o_point_2, o_point_3) ||
		point_in_triangle(t_point_4, o_point_1, o_point_2, o_point_4) ||
		point_in_triangle(o_point_1, t_point_1, t_point_2, t_point_3) ||
		point_in_triangle(o_point_1, t_point_1, t_point_2, t_point_4) ||
		point_in_triangle(o_point_2, t_point_1, t_point_2, t_point_3) ||
		point_in_triangle(o_point_2, t_point_1, t_point_2, t_point_4) ||
		point_in_triangle(o_point_3, t_point_1, t_point_2, t_point_3) ||
		point_in_triangle(o_point_3, t_point_1, t_point_2, t_point_4) ||
		point_in_triangle(o_point_4, t_point_1, t_point_2, t_point_3) ||
		point_in_triangle(o_point_4, t_point_1, t_point_2, t_point_4))
	{
		std::cout << "Kolizja!" << std::endl;
		return true;
	}
	else {
		return false;
	}
		
}

void Truck::change_angle(float value) {
	this->steer_angle += value;
	//std::cout << "Steer angle: " << steer_angle << std::endl;
	if (this->steer_angle > 50.0f)
		this->steer_angle = 50.0f;
	if (this->steer_angle < -50.0f)
		this->steer_angle = -50.0f;
}
