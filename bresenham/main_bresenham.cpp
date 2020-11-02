/*---------------------HEAD NOTES---------------------------*/
/*-------------------Bresenham Algorithm--------------------*/
/*-----------------------2021-1-----------------------------*/
/*-------Alumno:--Alfonso Murrieta Villegas ----------------*/

/*NOTA: El presente c�digo est� en gran medida basado en el material
		de laboratorio del profesor Luis Sergio Valencia Castro, 
		pr�ctica 3 y 4. 
*/



/*---------------------LIBRARIES---------------------------*/

// Para  movimiento en viewport
#include <glew.h>
#include <glfw3.h>

// Para gr�ficos
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader_m.h>

// Para el manejo de datos
#include <vector>

/*---------------------FUNCTIONS CALL---------------------------*/
void getResolution(void);
void resize(GLFWwindow* window, int width, int height);
void bresenhamAlgorithm(int, float, float);
void bresenhamComplement(float x, float y);
void axesPoints(float, float);
void pointsData(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b);
void myData(void);
void drawing(Shader);

/*---------------------GLOBAL VARIABLES---------------------------*/
//Graphics
GLFWmonitor *monitors;
GLuint VBO, VAO, EBO; //Vertex Buffer/Array/ Object y Element Buffer Object

int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

//para el dibujo de los puntos
std::vector<GLfloat> points;

//para el calculo de los puntos
std::vector<float> auxPointsX;
std::vector<float> auxPointsY;

//variables para la vista, se usan tambien para moverse con teclado
float movX = 0.0f;
float movY = 0.0f;
float movZ = 0.0f;// Mover el objeto en z para poder dimensionarlo a 3/4 en el viewport


/*---------------------WINDOWS FUNCTIONS---------------------------*/

//Funciones para el viewport y tama�o deventana
void getResolution(){
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void resize(GLFWwindow* window, int width, int height) {
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
}

/*---------------------ALGORITHM FUNCTIONS---------------------------*/

void bresenhamAlgorithm(int radio, float coorX, float coorY) { //funcion para dibujar circunferencia 
	//declaramos las variables para primer punto
	int auxX = 0;
	int y = radio;
	int p = 1 - radio;

	//variables para la matriz view del display, para ajustar la circunferencia aprox a 3/4 de la pantalla de ancho y centrarla despues de calcular cada punto 
	//para tener un resultado homogeneo
	movZ = -radio * 3;
	movX = movX - coorX;
	movY = movY - coorY;

	auxPointsX.push_back(auxX);
	auxPointsY.push_back(y);
	while (auxX <= y ) {
		if (p < 0) {
			auxX++;
			p = p + (2*auxX) + 3;
		}
		else {
			auxX++;
			y--;
			p = p + (2 * auxX) - (2 * y) + 5;
		}
		auxPointsX.push_back(auxX);
		auxPointsY.push_back(y);
	}

	//Obtenci�n de los dem�s puntos
	bresenhamComplement((float)coorX,(float)coorY);
	//Puntos de los ejes
	axesPoints((float)coorX, (float)coorY);
}


void bresenhamComplement(float x, float y) {

	/*
	ESQUEMA DE CICLOS FOR PARA LOS CUADRANTES
	(Sentido horario)

				|
		 4  	|    1
				|
	 -----------|-----------
				|
		 3  	|    2
				|

	
	*/			


	//Cuadrante 1
	for (int i = 0; i < auxPointsX.size() - 1; i++) {
		pointsData((GLfloat)(x + auxPointsX[i]), (GLfloat)(y + auxPointsY[i]), (GLfloat)0.0f, (GLfloat) 1.0f, (GLfloat)0.0f, (GLfloat)0.0f);
	}
	for (int i = auxPointsX.size() - 1; i >= 0; i--) {
		pointsData((GLfloat)(x + auxPointsY[i]), (GLfloat)(y + auxPointsX[i]), (GLfloat)0.0f, (GLfloat) 1.0f, (GLfloat)0.0f, (GLfloat)0.0f);
	}


	//Cuadrante 2
	for (int i = 0; i < auxPointsX.size() - 1; i++) {
		pointsData((GLfloat)(x + auxPointsY[i]), (GLfloat)(y - auxPointsX[i]), (GLfloat)0.0f, (GLfloat) 1.0f, (GLfloat)0.0f, (GLfloat)0.0f);
	}
	for (int i = auxPointsX.size() - 1; i >= 0; i--) {
		pointsData((GLfloat)(x + auxPointsX[i]), (GLfloat)(y - auxPointsY[i]), (GLfloat)0.0f, (GLfloat) 1.0f, (GLfloat)0.0f, (GLfloat)0.0f);
	}

	//Cuadrante 3
	for (int i = 0; i < auxPointsX.size() - 1; i++) {
		pointsData((GLfloat)(x - auxPointsX[i]), (GLfloat)(y - auxPointsY[i]), (GLfloat)0.0f, (GLfloat) 1.0f, (GLfloat)0.0f, (GLfloat)0.0f);
	}
	for (int i = auxPointsX.size() - 1; i >= 0; i--) {
		pointsData((GLfloat)(x - auxPointsY[i]), (GLfloat)(y - auxPointsX[i]), (GLfloat)0.0f, (GLfloat) 1.0f, (GLfloat)0.0f, (GLfloat)0.0f);
	}

	//Cuadrante 4
	for (int i = 0; i < auxPointsX.size() - 1; i++) {
		pointsData((GLfloat)(x - auxPointsY[i]), (GLfloat)(y + auxPointsX[i]), (GLfloat)0.0f, (GLfloat) 1.0f, (GLfloat)0.0f, (GLfloat)0.0f);
	}
	for (int i = auxPointsX.size() - 1; i >= 0; i--) {
		pointsData((GLfloat)(x - auxPointsX[i]), (GLfloat)(y + auxPointsY[i]), (GLfloat)0.0f, (GLfloat) 1.0f, (GLfloat)0.0f, (GLfloat)0.0f);
	}
}

void axesPoints(float x, float y) {
	//Eje X
	pointsData(-5000.0f, 0.0f, -0.1f,1.0f, 1.0f, 1.0f);
	pointsData(5000.0f, 0.0f, -0.1f, 1.0f, 1.0f, 1.0f);

	// eje Y
	pointsData(0.0f, -5000.0f, -0.1f, 1.0f, 1.0f, 1.0f);
	pointsData(0.0f, 5000.0f, -0.1f, 1.0f, 1.0f, 1.0f);
}


/*---------------------DRAWING FUNCTIONS---------------------------*/

void pointsData(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b) {
	points.push_back(x);
	points.push_back(y);
	points.push_back(z);

	//For colors
	points.push_back(r);
	points.push_back(g);
	points.push_back(b);
}

void myData(){
		
	//NOTA: Se copian los datos dados por Bresenham al buffer
	GLfloat vertices[100000];
	std::copy(points.begin(), points.end(), vertices); 

	unsigned int indices[] ={
		0
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Buffer
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Element Buffer Object
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void drawing(Shader shader){

	//Resources
	shader.use();
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);	


	//MANIPULACI�N Y VISTA

	//Para poder escalar la circunferencia respecto a la ventana emple� una proyecci�n
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);

	// Mediante una traslaci�n centramos el modelo en la vista de la ventana
	view = glm::translate(view, glm::vec3(movX, movY, movZ));
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	glBindVertexArray(VAO);
	

	//TRAZADO DE LAS ENTIDADES

	//Trazado de ejes cartesianos
	shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
	glDrawArrays(GL_LINES, (auxPointsX.size() * 8) - 4, (auxPointsX.size() * 8) - 2);

	//Trazado de la circunferencia
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
	glDrawArrays(GL_POLYGON, 0, (auxPointsX.size()*8)-4); 
	glBindVertexArray(0);
}


/*---------------------GENERAL FUNCTIONS---------------------------*/

void consoleInput(){
	int radio = 0;
	float x, y;

	std::cout << "Circunferencia por algoritmo de Bresenham" << "\n";
	std::cout << "Ingrese el radio (No mayor a mil): ";
	std::cin >> radio;

	if (radio>1 and radio < 1000){
		std::cout << "Coordenadas centro circunferencia: " << "\n" << "[x]: ";
		std::cin >> x;
		std::cout << "[y]: ";
		std::cin >> y;

		bresenhamAlgorithm(radio, x, y);
	}
	else{
		std::cout << "Valor no v�lido para el radio ";
	}

}

int main(int argc, char** argv){

    // glfw: initialize and configure
    glfwInit();

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
	monitors = glfwGetPrimaryMonitor();
	getResolution();

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Bresenham Algorithm ", NULL, NULL);

    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwSetWindowPos(window, 0, 30);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize);

	glewInit();

	//Llamada de validaci�n y algoritmo <- Parte agregada al c�digo del profesor Valencia
	consoleInput();
	myData();

	glEnable(GL_DEPTH_TEST);
	Shader projectionShader("shaders/shader_projection.vs", "shaders/shader_projection.fs");

    // render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window)){

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

        // render
        // Backgound color
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		drawing(projectionShader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}




