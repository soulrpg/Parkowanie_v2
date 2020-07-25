#include "Basic.h"


Basic::Basic(Model_type model_type, glm::vec3 transform, glm::vec3 scale, std::string texture_name, float rotation_angle, glm::vec3 rotation, bool is_visible, bool is_rotating){
	this->model_type = model_type;
	this->transform = transform;
	this->scale = scale;
	this->rotation = rotation;
	this->texture_name = texture_name;
	this->rotation_angle = rotation_angle;
	this->is_visible = is_visible;
	this->is_rotating = is_rotating;
	texture_handle = readTexture((char*)("Textures/" + texture_name + ".png").c_str());
}

void Basic::draw() {
	if (this->is_visible)
	{
		Basic::sp->use();
		glm::mat4 M = glm::mat4(1.0f);
		M = glm::translate(M, transform);
		M = glm::rotate(M, glm::radians(rotation_angle), rotation);
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
	}
}

//Funkcja wczytuj¹ca teksturê 
GLuint Basic::readTexture(char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Wczytanie do pamiêci komputera
	std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
	unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);

	//Import do pamiêci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pamiêci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}

void Basic::translate(glm::vec3 pos) {
	this->transform = pos;
}

void Basic::updateRotation() {
	if (is_rotating) {
		//angle_x += speed_x * glfwGetTime(); //Oblicz k¹t o jaki obiekt obróci³ siê podczas poprzedniej klatki
		//angle_y += speed_y * glfwGetTime(); //Oblicz k¹t o jaki obiekt obróci³ siê podczas poprzedniej klatki
		rotation_angle += 3.0f * glfwGetTime();
	}
}
