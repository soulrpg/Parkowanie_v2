#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "constants.h"
#include "shaderprogram.h"
#include "lodepng.h"

namespace model_types {
	enum Model_type {
		jeep, slupek, lamp, truck, trailer, arrow
	};
}
using namespace model_types;


class Basic
{
public:
	static const int num_of_unique_models = 6;
	static int* vertexCount[num_of_unique_models];
	static float* vertices[num_of_unique_models];
	static float* texCoords[num_of_unique_models];
	static float* normals[num_of_unique_models];
private:
	glm::vec4 scale;
	glm::mat4 M;
	float rotation_angle[3];
	std::string texture_name;
	bool is_rotating[3];
	Model_type model_type;
	bool is_visible;
	GLuint texture_handle;
	ShaderProgram* sp;

public:
	Basic(Model_type model_type, glm::mat4 M, glm::vec4 scale, std::string texture_name, float rotation_angle[], bool is_rotating[], bool is_visible);
	void draw();
	static GLuint readTexture(char* filename);
};



