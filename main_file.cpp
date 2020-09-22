/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "myCube.h"
#include <iostream>
#include "Models/jeep.h"
#include "Models/slupeknew.h"
#include "Models/lampnew.h"
#include "Models/trucknew.h"
#include "Models/trailernew.h"
#include "Models/arrownew.h"
#include "Models/diamondnew.h"
#include "Models/goblet.h"
#include "Models/MapleTreeLeaves.h"
#include "Models/MapleTreeStem.h"
#include "Camera.h"
#include "Basic.h"
#include "Arrow.h"
#include "Models/wheel.h"
#include "cube.h"
#include "Truck.h"
#include "Sky.h"


// Ilosc modeli na scenie
#define NUM_OF_MODELS_TOTAL 60
#define NUM_OF_BOXES 10

// Miejsce w tablicy modeli, w ktorym znajduje sie ciezarowka (potrzebne do polimorfizmu)
#define TRUCK_ID 1
#define GROUND_ID 2
#define ARROW_ID 46
#define DIAMOND_1_ID 47
#define DIAMOND_2_ID 48
#define GOBLET_ID 49

Camera* camera;

// Stan gry - aktywuje sie gdy zmienia sie stan, aby zmienic polozenie paru obiektow
bool state_changed = true;

// 0 - Poczatek, 1 - Po pierwszym parkowaniu, 2 - Po drugim parkowaniu, 3 - Zwyciestwo 
int game_state = 0;

// Ilosc unikalnych modeli (typow)
int Basic::num_of_unique_models = 12;

int* Basic::vertexCount = new int[num_of_unique_models];
float** Basic::vertices = new float*[num_of_unique_models];
float** Basic::normals = new float*[num_of_unique_models];
float** Basic::texCoords = new float*[num_of_unique_models];

bool W = false;
bool S = false;
bool A = false;
bool D = false;

bool arrow_down = false;
bool arrow_up = false;
bool arrow_left = false;
bool arrow_right = false;

Basic* models[NUM_OF_MODELS_TOTAL];
Sky* boxes[NUM_OF_BOXES];

ShaderProgram* Basic::sp;

float LightPos[] = {
	-27.0f, 5.0f, -42.0f, 1.0f,
	54.0f, 5.0f, -50.0f, 1.0f,
	54.0f, 5.0f, 45.0f, 1.0f,
	26.0f, 5.0f, -2.0f, 1.0f,
};

//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void key_callback(
	GLFWwindow* window,
	int key,
	int scancode,
	int action,
	int mod
) {
	if (game_state != 3) {
		if (key == GLFW_KEY_W && action == GLFW_PRESS) {
			std::cout << "W pressed!" << std::endl;
			W = true;
		}
		if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
			std::cout << "W released!" << std::endl;
			W = false;
		}
		if (key == GLFW_KEY_A && action == GLFW_PRESS) {
			std::cout << "A pressed!" << std::endl;
			A = true;
		}
		if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
			std::cout << "A released!" << std::endl;
			A = false;
		}
		if (key == GLFW_KEY_S && action == GLFW_PRESS) {
			std::cout << "S pressed!" << std::endl;
			S = true;
		}
		if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
			std::cout << "S released!" << std::endl;
			S = false;
		}
		if (key == GLFW_KEY_D && action == GLFW_PRESS) {
			std::cout << "D pressed!" << std::endl;
			D = true;
		}
		if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
			std::cout << "D released!" << std::endl;
			D = false;
		}
		if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
			std::cout << "UP pressed" << std::endl;
			arrow_up = true;
		}
		if (key == GLFW_KEY_UP && action == GLFW_RELEASE) {
			std::cout << "UP released!" << std::endl;
			arrow_up = false;
		}
		if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
			std::cout << "DOWN pressed" << std::endl;
			arrow_down = true;
		}
		if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE) {
			std::cout << "DOWN released!" << std::endl;
			arrow_down = false;
		}
		if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
			std::cout << "LEFT pressed" << std::endl;
			arrow_left = true;
		}
		if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE) {
			std::cout << "LEFT released!" << std::endl;
			arrow_left = false;
		}
		if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
			std::cout << "RIGHT pressed" << std::endl;
			arrow_right = true;
		}
		if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE) {
			std::cout << "RIGHT released!" << std::endl;
			arrow_right = false;
		}
	}
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	//std::cout << "Mouse pos X: " << xpos << std::endl << "Mouse pos Y: " << ypos << std::endl;
	// Przekazuje do funkcji move() kamery o ile pixeli ruszylismy mysza w danym kierunku
	if (camera->prev_mouse_x != -1 && camera->prev_mouse_y != -1) {
		camera->change_angle(camera->prev_mouse_x - xpos, camera->prev_mouse_y - ypos);
	}
	camera->prev_mouse_x = xpos;
	camera->prev_mouse_y = ypos;
}

void mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		camera->is_moving = true;
		camera->backwards = false;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		camera->is_moving = true;
		camera->backwards = true;
	}
	else if ((button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) || (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)) {
		camera->is_moving = false;
		camera->backwards = false;
	}
}

void load_models() {
	std::cout << "Loading models..." << std::endl;
	Basic::vertexCount[jeep] = jeepVertexCount;
	Basic::vertexCount[slupek] = slupeknewVertexCount;
	Basic::vertexCount[lamp] = lampnewVertexCount;
	Basic::vertexCount[truck] = trucknewVertexCount;
	Basic::vertexCount[trailer] = trailernewVertexCount;
	Basic::vertexCount[arrow] = arrownewVertexCount;
	Basic::vertexCount[wheel] = wheelVertexCount;
	Basic::vertexCount[diamond] = diamondnewVertexCount;
	Basic::vertexCount[goblet] = gobletVertexCount;
	Basic::vertexCount[cube] = Models::CubeInternal::vertexCount;
	Basic::vertexCount[stem] = MapleTreeStemVertexCount;
	Basic::vertexCount[leaves] = MapleTreeLeavesVertexCount;

	Basic::vertices[jeep] = jeepVertices;
	Basic::vertices[slupek] = slupeknewVertices;
	Basic::vertices[lamp] = lampnewVertices;
	Basic::vertices[truck] = trucknewVertices;
	Basic::vertices[trailer] = trailernewVertices;
	Basic::vertices[arrow] = arrownewVertices;
	Basic::vertices[wheel] = wheelVertices;
	Basic::vertices[diamond] = diamondnewVertices;
	Basic::vertices[goblet] = gobletVertices;
	Basic::vertices[cube] = Models::CubeInternal::vertices;
	Basic::vertices[stem] = MapleTreeStemVertices;
	Basic::vertices[leaves] = MapleTreeLeavesVertices;

	Basic::normals[jeep] = jeepNormals;
	Basic::normals[slupek] = slupeknewNormals;
	Basic::normals[lamp] = lampnewNormals;
	Basic::normals[truck] = trucknewNormals;
	Basic::normals[trailer] = trailernewNormals;
	Basic::normals[arrow] = arrownewNormals;
	Basic::normals[wheel] = wheelNormals;
	Basic::normals[diamond] = diamondnewNormals;
	Basic::normals[goblet] = gobletNormals;
	Basic::normals[cube] = Models::CubeInternal::normals;
	Basic::normals[stem] = MapleTreeStemNormals;
	Basic::normals[leaves] = MapleTreeLeavesNormals;

	Basic::texCoords[jeep] = jeepTexCoords;
	Basic::texCoords[slupek] = slupeknewTexCoords;
	Basic::texCoords[lamp] = lampnewTexCoords;
	Basic::texCoords[truck] = trucknewTexCoords;
	Basic::texCoords[trailer] = trailernewTexCoords;
	Basic::texCoords[arrow] = arrownewTexCoords; 
	Basic::texCoords[wheel] = wheelTexCoords; 
	Basic::texCoords[diamond] = diamondnewTexCoords; 
	Basic::texCoords[goblet] = gobletTexCoords; 
	Basic::texCoords[cube] = Models::CubeInternal::texCoords;
	Basic::texCoords[stem] = MapleTreeStemTexCoords;
	Basic::texCoords[leaves] = MapleTreeLeavesTexCoords;
	std::cout << "Models loaded." << std::endl;
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
    initShaders();
	//Basic::sp = spLambertTextured;
	Basic::sp = new ShaderProgram("v_shader.glsl", NULL, "f_shader.glsl");
	load_models();
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	camera = new Camera(glm::vec3(0.0f, 1.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // Tworzy obiekt kamery

	// Inicjalizuje modele na scenie
	models[0] = new Basic(jeep, glm::vec3(0.0f, 0.1f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), "jeep", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[1] = new Truck(truck, glm::vec3(15.0f, -0.3f, 0.0f), glm::vec3(0.09f, 0.09f, 0.09f), "trucknew", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f),
		true, false, glm::vec3(1.45f, 0.9f, 3.2f), glm::vec3(-0.85f, 0.9f, 3.2f));
	models[2] = new Basic(cube, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(75.0f, 0.05f, 75.0f), "podloze", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[3] = new Basic(jeep, glm::vec3(0.0f, 0.1f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), "jeep", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[4] = new Basic(cube, glm::vec3(19.0f, 0.5f, 12.0f), glm::vec3(9.0f, 0.5f, 1.0f), "curb", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[5] = new Basic(cube, glm::vec3(19.0f, 0.5f, -17.0f), glm::vec3(9.0f, 0.5f, 1.0f), "curb", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[6] = new Basic(cube, glm::vec3(-17.0f, 0.5f, 19.0f), glm::vec3(11.8f, 0.5f, 1.0f), "curb", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[7] = new Basic(cube, glm::vec3(-36.0f, 0.5f, -10.0f), glm::vec3(8.0f, 0.5f, 1.0f), "curb", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[8] = new Basic(cube, glm::vec3(-28.0f, 0.5f, 4.3f), glm::vec3(1.0f, 0.5f, 13.5f), "curb", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[9] = new Basic(cube, glm::vec3(28.0f, 0.5f, -3.0f), glm::vec3(1.0f, 0.5f, 14.0f), "curb", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[10] = new Basic(cube, glm::vec3(16.0f, 0.5f, 41.0f), glm::vec3(22.0f, 0.5f, 1.0f), "curb", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[11] = new Basic(cube, glm::vec3(33.0f, 0.5f, 25.0f), glm::vec3(22.0f, 0.5f, 1.0f), "curb", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[12] = new Basic(cube, glm::vec3(0.0f, 0.5f, -39.0f), glm::vec3(26.0f, 0.5f, 1.0f), "curb", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[13] = new Basic(cube, glm::vec3(32.0f, 0.5f, -23.0f), glm::vec3(22.0f, 0.5f, 1.0f), "curb", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[14] = new Basic(cube, glm::vec3(-35.0f, 0.5f, -53.0f), glm::vec3(9.0f, 0.5f, 1.0f), "curb", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[15] = new Basic(cube, glm::vec3(48.0f, 0.5f, 66.0f), glm::vec3(9.0f, 0.5f, 1.0f), "curb", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[16] = new Basic(cube, glm::vec3(41.0f, 0.5f, -61.0f), glm::vec3(16.0f, 0.5f, 1.0f), "curb", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[17] = new Basic(cube, glm::vec3(61.0f, 0.5f, -32.0f), glm::vec3(6.0f, 0.5f, 1.0f), "curb", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[18] = new Basic(cube, glm::vec3(61.0f, 0.5f, -50.0f), glm::vec3(6.0f, 0.5f, 1.0f), "curb", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[19] = new Basic(cube, glm::vec3(10.0f, 0.5f,  19.1f), glm::vec3(1.0f, 0.5f, 6.0f), "curb", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[20] = new Basic(cube, glm::vec3(-6.0f, 0.5f, 30.0f), glm::vec3(1.0f, 0.5f, 10.0f), "curb", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[21] = new Basic(cube, glm::vec3(-43.0f, 0.5f, -31.3f), glm::vec3(1.0f, 0.5f, 20.5f), "curb", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[22] = new Basic(cube, glm::vec3(-25.0f, 0.5f, -47.0f), glm::vec3(1.0f, 0.5f, 7.0f), "curb", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[23] = new Basic(cube, glm::vec3(38.0f, 0.5f, 54.0f), glm::vec3(1.0f, 0.5f, 12.0f), "curb", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[24] = new Basic(cube, glm::vec3(56.0f, 0.5f, 45.0f), glm::vec3(1.0f, 0.5f, 20.0f), "curb", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[25] = new Basic(cube, glm::vec3(11.0f, 0.5f, -20.0f), glm::vec3(1.0f, 0.5f, 2.2f), "curb", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[26] = new Basic(cube, glm::vec3(26.0f, 0.5f, -50.0f), glm::vec3(1.0f, 0.5f, 10.0f), "curb", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[27] = new Basic(cube, glm::vec3(55.0f, 0.5f, -27.0f), glm::vec3(1.0f, 0.5f, 4.0f), "curb", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[28] = new Basic(cube, glm::vec3(68.0f, 0.5f, -41.0f), glm::vec3(1.0f, 0.5f, 10.0f), "curb", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[29] = new Basic(cube, glm::vec3(56.0f, 0.5f, -55.5f), glm::vec3(1.0f, 0.5f, 4.5f), "curb", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[30] = new Basic(lamp, glm::vec3(-27.0f, 0.1f, -42.0f), glm::vec3(2.0f, 2.0f, 2.0f), "lampnew", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[31] = new Basic(lamp, glm::vec3(54.0f, 0.1f, -50.0f), glm::vec3(2.0f, 2.0f, 2.0f), "lampnew", 90.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[32] = new Basic(lamp, glm::vec3(54.0f, 0.1f, 45.0f), glm::vec3(2.0f, 2.0f, 2.0f), "lampnew", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[33] = new Basic(lamp, glm::vec3(26.0f, 0.1f, -2.0f), glm::vec3(2.0f, 2.0f, 2.0f), "lampnew", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[34] = new Basic(jeep, glm::vec3(-40.0f, 0.1f, -44.0f), glm::vec3(1.0f, 1.0f, 1.0f), "jeep", 180.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[35] = new Basic(jeep, glm::vec3(30.0f, 0.1f, -56.0f), glm::vec3(1.0f, 1.0f, 1.0f), "jeep", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[36] = new Basic(jeep, glm::vec3(60.0f, 0.1f, -41.0f), glm::vec3(1.0f, 1.0f, 1.0f), "jeep", 90.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[37] = new Basic(jeep, glm::vec3(52.0f, 0.1f, 57.0f), glm::vec3(1.0f, 1.0f, 1.0f), "jeep", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[38] = new Basic(trailer, glm::vec3(46.0f, 0.1f, 58.0f), glm::vec3(0.8f, 0.8f, 0.8f), "trailernew", 180.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[39] = new Basic(trailer, glm::vec3(5.0f, 0.1f, 0.0f), glm::vec3(0.8f, 0.8f, 0.8f), "trailernew", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[40] = new Basic(slupek, glm::vec3(29.0f, 1.4f, 32.0f), glm::vec3(0.2f, 0.2f, 0.2f), "slupeknew", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, true);
	models[41] = new Basic(slupek, glm::vec3(-25.0f, 1.4f, -32.0f), glm::vec3(0.2f, 0.2f, 0.2f), "slupeknew", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, true);
	models[42] = new Basic(slupek, glm::vec3(-5.0f, 1.4f, -31.0f), glm::vec3(0.2f, 0.2f, 0.2f), "slupeknew", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, true);
	models[43] = new Basic(slupek, glm::vec3(4.0f, 1.4f, -20.0f), glm::vec3(0.2f, 0.2f, 0.2f), "slupeknew", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, true);
	models[44] = new Basic(slupek, glm::vec3(-26.0f, 1.4f, -16.0f), glm::vec3(0.2f, 0.2f, 0.2f), "slupeknew", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, true);
	models[45] = new Basic(slupek, glm::vec3(40.0f, 1.4f, -40.0f), glm::vec3(0.2f, 0.2f, 0.2f), "slupeknew", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, true);
	models[46] = new Arrow(arrow, glm::vec3(-35.0f, 6.0f, -45.0f), glm::vec3(1.5f, 1.5f, 1.5f), "arrownew", -90.0f, glm::vec3(1.0f, 0.0f, 0.0f), true, false);
	models[47] = new Basic(diamond, glm::vec3(-35.0f, 0.1f, -42.0f), glm::vec3(0.004f, 0.004f, 0.004f), "diamond", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, true);
	models[48] = new Basic(diamond, glm::vec3(-35.0f, 0.1f, -49.0f), glm::vec3(0.004f, 0.004f, 0.004f), "diamond", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, true);
	models[49] = new Basic(goblet, glm::vec3(38.0f, 1.5f, 60.0f), glm::vec3(0.4f, 0.4f, 0.4f), "goblet", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), false, true);
	models[50] = new Basic(stem, glm::vec3(35.0f, 0.5f, 2.0f), glm::vec3(0.3f, 0.3f, 0.3f), "maple_bark", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[51] = new Basic(leaves, glm::vec3(35.0f, 0.5f, 2.0f), glm::vec3(0.3f, 0.3f, 0.3f), "maple_leaf", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[52] = new Basic(stem, glm::vec3(40.0f, 0.5f, -3.0f), glm::vec3(0.4f, 0.4f, 0.4f), "maple_bark", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[53] = new Basic(leaves, glm::vec3(40.0f, 0.5f, -3.0f), glm::vec3(0.4f, 0.4f, 0.4f), "maple_leaf", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[54] = new Basic(stem, glm::vec3(-50.0f, 0.5f, 7.0f), glm::vec3(0.4f, 0.4f, 0.4f), "maple_bark", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[55] = new Basic(leaves, glm::vec3(-50.0f, 0.5f, 7.0f), glm::vec3(0.4f, 0.4f, 0.4f), "maple_leaf", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[56] = new Basic(stem, glm::vec3(-50.0f, 0.5f, -40.0f), glm::vec3(0.35f, 0.35f, 0.35f), "maple_bark", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[57] = new Basic(leaves, glm::vec3(-50.0f, 0.5f, -40.0f), glm::vec3(0.35f, 0.35f, 0.35f), "maple_leaf", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[58] = new Basic(stem, glm::vec3(-40.0f, 0.5f, 20.0f), glm::vec3(0.4f, 0.4f, 0.4f), "maple_bark", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[59] = new Basic(leaves, glm::vec3(-40.0f, 0.5f, 20.0f), glm::vec3(0.4f, 0.4f, 0.4f), "maple_leaf", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);

	boxes[0] = new Sky(cube, glm::vec3(500.0f, 0.0f, 0.0f), glm::vec3(0.1f, 502.0f, 502.0f), "skybox_left", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	boxes[1] = new Sky(cube, glm::vec3(-500.0f, 0.0f, 0.0f), glm::vec3(0.1f, 502.0f, 502.0f), "skybox_right", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	boxes[2] = new Sky(cube, glm::vec3(0.0f, 0.0f, 500.0f), glm::vec3(502.0f, 502.0f, 0.1f), "skybox_back", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	boxes[3] = new Sky(cube, glm::vec3(0.0f, 0.0f, -500.0f), glm::vec3(502.0f, 502.0f, 0.1f), "skybox_middle", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	boxes[4] = new Sky(cube, glm::vec3(0.0f, 500.0f, 0.0f), glm::vec3(502.0f, 0.1f, 502.1f), "skybox_top", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	boxes[5] = new Sky(cube, glm::vec3(0.0f, -500.0f, 0.0f), glm::vec3(502.0f, 0.1f, 502.1f), "skybox_bottom", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	
	boxes[6] = new Sky(cube, glm::vec3(75.0f, 0.0f, 0.0f), glm::vec3(0.4f, 10.0f, 75.0f), "wall", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	boxes[7] = new Sky(cube, glm::vec3(-75.0f, 0.0f, 0.0f), glm::vec3(0.4f, 10.0f, 75.0f), "wall", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	boxes[8] = new Sky(cube, glm::vec3(0.0f, 0.0f, 75.0f), glm::vec3(75.0f, 10.0f, 0.4f), "wall", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	boxes[9] = new Sky(cube, glm::vec3(0.0f, 0.0f, -75.0f), glm::vec3(75.0f, 10.0f, 0.4f), "wall", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	// Koniec inicjalizacji modeli (ilość modeli w NUM_OF_MODELS_TOTAL
	
	glClearColor(1.0f, 1.0f, 0, 1); //Ustaw kolor czyszczenia bufora kolorów
	glEnable(GL_DEPTH_TEST); //Włącz test głębokości na pikselach
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    freeShaders();
    //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
	//glDeleteTextures(1, &tex);
	delete camera;
	delete Basic::sp;
	delete[] Basic::vertexCount;
	delete[] Basic::vertices;
	delete[] Basic::normals;
	delete[] Basic::texCoords;
	//delete[] models;
}

void drawPoint(glm::vec4 point) {
	glm::vec3 point_3 = glm::vec3(point.x, point.y, point.z);
	Basic::sp->use();
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::translate(M, point_3);
	M = glm::scale(M, glm::vec3(0.2f, 0.2f, 0.2f));
	glUniformMatrix4fv(Basic::sp->u("M"), 1, false, glm::value_ptr(M));
	glEnableVertexAttribArray(Basic::sp->a("vertex"));
	glVertexAttribPointer(Basic::sp->a("vertex"), 4, GL_FLOAT, false, 0, Basic::vertices[cube]);

	glEnableVertexAttribArray(Basic::sp->a("normal"));
	glVertexAttribPointer(Basic::sp->a("normal"), 4, GL_FLOAT, false, 0, Basic::normals[cube]);

	glDrawArrays(GL_TRIANGLES, 0, Basic::vertexCount[cube]);
	glDisableVertexAttribArray(Basic::sp->a("vertex"));
	glDisableVertexAttribArray(Basic::sp->a("normal"));
}


//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glm::mat4 V = glm::lookAt(camera->position, camera->position + camera->to_target_vector, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 P = glm::perspective(glm::radians(50.0f), float(WIDTH/HEIGHT), 1.0f, 1000.0f);

	// Kod rysujący
	Basic::sp->use();
	glUniformMatrix4fv(Basic::sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(Basic::sp->u("V"), 1, false, glm::value_ptr(V));

	glUniform4fv(Basic::sp->u("lp"), 4, LightPos);

	// Rysowanie modeli
	for (int i = 0; i < NUM_OF_MODELS_TOTAL; i++) {
		models[i]->draw(); 
	}
	
	spLambertTextured->use();
	glUniformMatrix4fv(spLambertTextured->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spLambertTextured->u("V"), 1, false, glm::value_ptr(V));

	for (int i = 0; i < NUM_OF_BOXES; i++) {
		boxes[i]->draw();
	}

	/*
	glm::mat4 t_matrix = models[TRUCK_ID]->getMMatrix();
	glm::vec4 point_1 = t_matrix * glm::vec4(((Truck*)models[TRUCK_ID])->max_vert_x, 5.0f, ((Truck*)models[TRUCK_ID])->max_vert_z, 1.0f);
	glm::vec4 point_2 = t_matrix * glm::vec4(((Truck*)models[TRUCK_ID])->min_vert_x, 5.0f, ((Truck*)models[TRUCK_ID])->max_vert_z, 1.0f);
	glm::vec4 point_3 = t_matrix * glm::vec4(((Truck*)models[TRUCK_ID])->min_vert_x, 5.0f, ((Truck*)models[TRUCK_ID])->min_vert_z, 1.0f);
	glm::vec4 point_4 = t_matrix * glm::vec4(((Truck*)models[TRUCK_ID])->max_vert_x, 5.0f, ((Truck*)models[TRUCK_ID])->min_vert_z, 1.0f);

	drawPoint(point_1);
	drawPoint(point_2);
	drawPoint(point_3);
	drawPoint(point_4);

	t_matrix = models[0]->getMMatrix();
	point_1 = t_matrix * glm::vec4(models[34]->max_vert_x, 5.0f, models[0]->max_vert_z, 1.0f);
	point_2 = t_matrix * glm::vec4(models[34]->min_vert_x, 5.0f, models[0]->max_vert_z, 1.0f);
	point_3 = t_matrix * glm::vec4(models[34]->min_vert_x, 5.0f, models[0]->min_vert_z, 1.0f);
	point_4 = t_matrix * glm::vec4(models[34]->max_vert_x, 5.0f, models[0]->min_vert_z, 1.0f);

	
	drawPoint(point_1);
	drawPoint(point_2);
	drawPoint(point_3);
	drawPoint(point_4);
	*/

    //glDisable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//Skopiuj bufor tylny do bufora przedniego
	glfwSwapBuffers(window); 

}

void sceneUpdate() {
	if (W == true) {
		if (((Truck*)models[TRUCK_ID])->acceleration <= 0.5f)
			((Truck*)models[TRUCK_ID])->acceleration += 0.05f;
	}

	if (S == true) {
		if (((Truck*)models[TRUCK_ID])->acceleration >= -0.5f)
			((Truck*)models[TRUCK_ID])->acceleration -= 0.05f;
	}
	if (W == false && S == false) {
		((Truck*)models[TRUCK_ID])->acceleration = 0.0f;
	}
	camera->update(arrow_up, arrow_down, arrow_left, arrow_right);
	((Truck*)models[TRUCK_ID])->movement();
	float angle_change = 0.0f;
	if (A == true) {
		angle_change = -100.0f;
	}
	if (D == true) {
		angle_change = 100.0f;
	}
	((Truck*)models[TRUCK_ID])->change_angle(angle_change * glfwGetTime());
	for (int i = 0; i < NUM_OF_MODELS_TOTAL; i++) {
		models[i]->updateRotation();
		if (i != TRUCK_ID && i!= GROUND_ID && i!= ARROW_ID && i!= DIAMOND_1_ID && i!= DIAMOND_2_ID) {
			if (((Truck*)models[TRUCK_ID])->collision(models[i])) {
				state_changed = true;
				std::cout << "Kolizja!" << std::endl;
				std::cout << i << std::endl;
				game_state = 0;
			}
		}
	}
	if (((Truck*)models[TRUCK_ID])->collision(models[DIAMOND_1_ID]) && ((Truck*)models[TRUCK_ID])->collision(models[DIAMOND_2_ID]) &&
		((Truck*)models[TRUCK_ID])->get_speed() == 0.0f) {
		game_state += 1;
		std::cout << "Nastepny stan!" << std::endl;
		state_changed = true;
	}
	if (state_changed) {
		state_changed = false;
		if (game_state == 0) {
			models[TRUCK_ID]->translate(glm::vec3(15.0f, -0.3f, 0.0f));
			models[TRUCK_ID]->set_rotation(0.0f);
			models[ARROW_ID]->translate(glm::vec3(-35.0f, 6.0f, -45.0f));
			models[DIAMOND_1_ID]->translate(glm::vec3(-35.0f, 0.1f, -42.0f));
			models[DIAMOND_2_ID]->translate(glm::vec3(-35.0f, 0.1f, -49.0f));
			((Truck*)models[TRUCK_ID])->set_speed(0.0f);
			((Truck*)models[TRUCK_ID])->acceleration = 0.0f;
			std::cout << "RESET!" << std::endl;
		}
		else if (game_state == 1) {
			models[ARROW_ID]->translate(glm::vec3(60.0f, 6.0f, -46.0f));
			models[DIAMOND_1_ID]->translate(glm::vec3(57.0f, 0.1f, -46.0f));
			models[DIAMOND_2_ID]->translate(glm::vec3(64.0f, 0.1f, -46.0f));
		}
		else if (game_state == 2) {
			models[ARROW_ID]->translate(glm::vec3(42.0f, 6.0f, 58.0f));
			models[DIAMOND_1_ID]->translate(glm::vec3(41.5f, 0.1f, 55.0f));
			models[DIAMOND_2_ID]->translate(glm::vec3(41.5f, 0.1f, 62.0f));
		}
		else {
			camera->set_final_position(96.1997f, 142.2f, glm::vec3(6.61f, -1.08f, -7.94f), glm::vec3(35.14f, 4.49f, 68.25f));
			models[ARROW_ID]->set_visibility(false);
			models[DIAMOND_1_ID]->set_visibility(false);
			models[DIAMOND_2_ID]->set_visibility(false);
			models[GOBLET_ID]->set_visibility(true);
			std::cout << "YOU WON!" << std::endl;
		}
	}
}


int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące

	//Główna pętla
	glfwSetTime(0); //Wyzeruj licznik czasu
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		sceneUpdate();
		glfwSetTime(0); //Wyzeruj licznik czasu
		drawScene(window); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
