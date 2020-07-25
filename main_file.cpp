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

// Ilosc modeli na scenie
#define NUM_OF_MODELS_TOTAL 2

Camera* camera;

// Ilosc unikalnych modeli (typow)
int Basic::num_of_unique_models = 6;

int* Basic::vertexCount = new int[num_of_unique_models];
float** Basic::vertices = new float*[num_of_unique_models];
float** Basic::normals = new float*[num_of_unique_models];
float** Basic::texCoords = new float*[num_of_unique_models];

Basic* models[NUM_OF_MODELS_TOTAL];

ShaderProgram* Basic::sp;


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
	}
	if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
		std::cout << "W released!" << std::endl;
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
	Basic::vertexCount[jeep] = jeepVertexCount;
	Basic::vertexCount[slupek] = slupeknewVertexCount;
	Basic::vertexCount[lamp] = lampnewVertexCount;
	Basic::vertexCount[truck] = trucknewVertexCount;
	Basic::vertexCount[trailer] = trailernewVertexCount;
	Basic::vertexCount[arrow] = arrownewVertexCount;

	Basic::vertices[jeep] = jeepVertices;
	Basic::vertices[slupek] = slupeknewVertices;
	Basic::vertices[lamp] = lampnewVertices;
	Basic::vertices[truck] = trucknewVertices;
	Basic::vertices[trailer] = trailernewVertices;
	Basic::vertices[arrow] = arrownewVertices;

	Basic::normals[jeep] = jeepNormals;
	Basic::normals[slupek] = slupeknewNormals;
	Basic::normals[lamp] = lampnewNormals;
	Basic::normals[truck] = trucknewNormals;
	Basic::normals[trailer] = trailernewNormals;
	Basic::normals[arrow] = arrownewNormals;

	Basic::texCoords[jeep] = jeepTexCoords;
	Basic::texCoords[slupek] = slupeknewTexCoords;
	Basic::texCoords[lamp] = lampnewTexCoords;
	Basic::texCoords[truck] = trucknewTexCoords;
	Basic::texCoords[trailer] = trailernewTexCoords;
	Basic::texCoords[arrow] = arrownewTexCoords; 
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
    initShaders();
	Basic::sp = spLambertTextured;
	load_models();
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	camera = new Camera(glm::vec3(0.0f, 1.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // Tworzy obiekt kamery

	// Inicjalizuje modele na scenie
	models[0] = new Basic(jeep, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), "jeep", 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, true);
	models[1] = new Basic(slupek, glm::vec3(2.0f, 0.2f, 0.0f), glm::vec3(0.05f, 0.05f, 0.05f), "slupeknew", 50.0f, glm::vec3(0.0f, 1.0f, 0.0f), true, false);
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
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glm::mat4 V = glm::lookAt(camera->position, camera->position + camera->to_target_vector, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 P = glm::perspective(glm::radians(50.0f), float(WIDTH/HEIGHT), 1.0f, 50.0f);

	// Kod rysujący
	Basic::sp->use();
	glUniformMatrix4fv(Basic::sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(Basic::sp->u("V"), 1, false, glm::value_ptr(V));

	// Rysowanie modeli
	for (int i = 0; i < NUM_OF_MODELS_TOTAL; i++) {
		models[i]->draw();
	}

    //glDisable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//Skopiuj bufor tylny do bufora przedniego
	glfwSwapBuffers(window); 

}

void sceneUpdate() {
	camera->update();
	for (int i = 0; i < NUM_OF_MODELS_TOTAL; i++) {
		models[i]->updateRotation();
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
