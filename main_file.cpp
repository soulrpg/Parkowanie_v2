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
#include "Camera.h"
#include "Basic.h"
#include "Arrow.h"
#include "Models/wheel.h"
#include "cube.h"
#include "Truck.h"


// Ilosc modeli na scenie
#define NUM_OF_MODELS_TOTAL 3

// Miejsce w tablicy modeli, w ktorym znajduje sie ciezarowka (potrzebne do polimorfizmu)
#define TRUCK_ID 1

Camera* camera;

// Ilosc unikalnych modeli (typow)
int Basic::num_of_unique_models = 8;

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

ShaderProgram* Basic::sp;

float LightPos[] = {
	20.0f, 0.0f, 0.0f, 1.0f,
	-20.0f, 0.0f, 0.0f, 1.0f
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
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
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
	Basic::vertexCount[jeep] = jeepVertexCount;
	Basic::vertexCount[slupek] = slupeknewVertexCount;
	Basic::vertexCount[lamp] = lampnewVertexCount;
	Basic::vertexCount[truck] = trucknewVertexCount;
	Basic::vertexCount[trailer] = trailernewVertexCount;
	Basic::vertexCount[arrow] = arrownewVertexCount;
	Basic::vertexCount[wheel] = wheelVertexCount;
	Basic::vertexCount[cube] = Models::CubeInternal::vertexCount;

	Basic::vertices[jeep] = jeepVertices;
	Basic::vertices[slupek] = slupeknewVertices;
	Basic::vertices[lamp] = lampnewVertices;
	Basic::vertices[truck] = trucknewVertices;
	Basic::vertices[trailer] = trailernewVertices;
	Basic::vertices[arrow] = arrownewVertices;
	Basic::vertices[wheel] = wheelVertices;
	Basic::vertices[cube] = Models::CubeInternal::vertices;

	Basic::normals[jeep] = jeepNormals;
	Basic::normals[slupek] = slupeknewNormals;
	Basic::normals[lamp] = lampnewNormals;
	Basic::normals[truck] = trucknewNormals;
	Basic::normals[trailer] = trailernewNormals;
	Basic::normals[arrow] = arrownewNormals;
	Basic::normals[wheel] = wheelNormals;
	Basic::normals[cube] = Models::CubeInternal::normals;

	Basic::texCoords[jeep] = jeepTexCoords;
	Basic::texCoords[slupek] = slupeknewTexCoords;
	Basic::texCoords[lamp] = lampnewTexCoords;
	Basic::texCoords[truck] = trucknewTexCoords;
	Basic::texCoords[trailer] = trailernewTexCoords;
	Basic::texCoords[arrow] = arrownewTexCoords; 
	Basic::texCoords[wheel] = wheelTexCoords; 
	Basic::texCoords[cube] = Models::CubeInternal::texCoords;
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
	models[0] = new Basic(jeep, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), "jeep", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
	models[1] = new Truck(truck, glm::vec3(15.0f, 0.0f, 0.0f), glm::vec3(0.09f, 0.09f, 0.09f), "trucknew", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f),
		true, false, glm::vec3(1.45f, 0.9f, 3.2f), glm::vec3(-0.85f, 0.9f, 3.2f));
	models[2] = new Basic(cube, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(50.0f, 0.05f, 50.0f), "", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
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
	glm::mat4 P = glm::perspective(glm::radians(50.0f), float(WIDTH/HEIGHT), 1.0f, 200.0f);

	// Kod rysujący
	Basic::sp->use();
	glUniformMatrix4fv(Basic::sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(Basic::sp->u("V"), 1, false, glm::value_ptr(V));

	glUniform4fv(Basic::sp->u("lp"), 2, LightPos);

	// Rysowanie modeli
	for (int i = 0; i < NUM_OF_MODELS_TOTAL; i++) {
		models[i]->draw(); 
	}

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
	point_1 = t_matrix * glm::vec4(models[0]->max_vert_x, 5.0f, models[0]->max_vert_z, 1.0f);
	point_2 = t_matrix * glm::vec4(models[0]->min_vert_x, 5.0f, models[0]->max_vert_z, 1.0f);
	point_3 = t_matrix * glm::vec4(models[0]->min_vert_x, 5.0f, models[0]->min_vert_z, 1.0f);
	point_4 = t_matrix * glm::vec4(models[0]->max_vert_x, 5.0f, models[0]->min_vert_z, 1.0f);


	drawPoint(point_1);
	drawPoint(point_2);
	drawPoint(point_3);
	drawPoint(point_4);

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
	((Truck*)models[TRUCK_ID])->collision(models[0]);
	/*
	for (int i = 0; i < NUM_OF_MODELS_TOTAL; i++) {
		models[i]->updateRotation();
		if (i != TRUCK_ID) {
			((Truck*)models[TRUCK_ID])->collision(models[i]);
		}
	}*/
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
