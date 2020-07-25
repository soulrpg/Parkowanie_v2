#include "Basic.h"


Basic::Basic(Model_type model_type, glm::mat4 M, glm::vec4 scale, std::string texture_name, float rotation_angle[], bool is_rotating[], bool is_visible){
	this->model_type = model_type;
	this->M = M;
	this->scale = scale;
	this->texture_name = texture_name;
	for (int i = 0; i < 3; i++) {
		this->rotation_angle[i] = rotation_angle[i];
		this->is_rotating[i] = is_rotating[i];
	}
	this->is_visible = is_visible;
	texture_handle = readTexture((char*)("Textures/" + texture_name + ".png").c_str());
	sp = new ShaderProgram("v_lamberttextured.glsl", NULL, "f_lamberttextured.glsl");
}

void Basic::draw() {
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(this->M));
	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, Basic::vertices[model_type]);

	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, Basic::normals[model_type]);

	glEnableVertexAttribArray(sp->a("texCoord"));
	glVertexAttribPointer(sp->a("texCoord"), 2, GL_FLOAT, false, 0, Basic::texCoords[model_type]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_handle);
	glUniform1i(sp->u("tex"), 0);
	glDrawArrays(GL_TRIANGLES, 0, *Basic::vertexCount[model_type]);
	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("normal"));
	glDisableVertexAttribArray(sp->a("texCoord"));
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