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
#include <iostream>

namespace model_types {
	enum Model_type {
		jeep, slupek, lamp, truck, trailer, arrow, cube, wheel, diamond
	};
}
using namespace model_types;


class Basic
{
public:
	static int num_of_unique_models;
	static int* vertexCount;
	static float** vertices;
	static float** texCoords;
	static float** normals;
	static ShaderProgram* sp;
	glm::vec3 transform;
protected:
	glm::vec3 scale;
	
	glm::vec3 rotation;
	float rotation_angle;
	std::string texture_name;
	GLuint texture_handle;
	Model_type model_type;
	bool is_visible;
	bool is_rotating;
public:
	float min_vert_x, max_vert_x, min_vert_z, max_vert_z;

public:
	Basic(Model_type model_type, glm::vec3 transform, glm::vec3 scale, std::string texture_name, float rotation_angle, glm::vec3 rotation, bool is_visible, bool is_rotating);
	virtual void draw();
	void translate(glm::vec3 pos);
	void applyScale();
	static GLuint readTexture(char* filename);
	void updateRotation();
	void setCorners();
	glm::mat4 getMMatrix();
};



