// Link statically with GLEW
#define GLEW_STATIC

// Headers
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include "common/loadShader.h"

using namespace std;

class ModelData {
public:
	//Constructor
	ModelData(string mn);
	//Deconstructor
	~ModelData();
	//Functions
	void loadData();
	void printData();
	//Public Variables
	string objData[10000];
protected:
	//Protected Variables
	string modelname;
};

ModelData::ModelData(string mn) { //Constructor, takes modelname as an argument
	modelname = mn;
}

ModelData::~ModelData() {
	//Destructor
}

void ModelData::loadData() {
	string line;
	int curLine;

	ifstream objFile("Assets/Models/" + modelname); //Opens file for reading
	if (!objFile.is_open()) {
		cout << "Sorry, the file could not be opened.";
	}
	else {
		curLine = 0;
		while (getline(objFile, line)) {
			if (curLine < 10000) {
				objData[curLine] = line;
				curLine++;
			}
		}
	}
	objFile.close();
}

void ModelData::printData() {
	for (int i = 0; i < 10000; i++) {
		if (objData[i] != "") {
			cout << objData[i] << "\n";
		}
	}
}

int main() {
	//Declare variables
	bool running = true;
	ModelData md("cube.obj");
	GLuint VertexArrayID;
	GLuint vertexbuffer;
	
	//Define points
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
	};

	//Open SFML window
	sf::Window window(sf::VideoMode(800, 600), "LL-Engine", sf::Style::Default, sf::ContextSettings(24U, 0U, 0U, 3U, 2U));

	//Init GLEW
	glewExperimental = true;
	glewInit();

	//OpenGL stuff
	glEnable(GL_TEXTURE_2D);

	//Generate and Bind Vertex Array
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	//Generate and Bind Vertex Buffer
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	
	md.loadData();
	md.printData();
	
	//Create and compile GLSL program from shaders
	GLuint programID = LoadShaders("Assets/Shaders/vertexShader.txt", "Assets/Shaders/fragmentShader.txt");

	while (running) {
		//Handle events
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				running = false;
			}
			else if (event.type == sf::Event::Resized) {
				glViewport(0, 0, event.size.width, event.size.height);
			}
		}

		//Clear buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Use shader
		glUseProgram(programID);

		//1st attribute buffer: vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0, //Attribute 0, no reason for 0 but must match shader layout
			3, //Size
			GL_FLOAT, //Type
			GL_FALSE, //If normalized or not
			0, //Stride
			(void*)0 //Array Buffer Offset
			);

		//Draw
		glDrawArrays(GL_TRIANGLES, 0, 3); //Starting from vertex 0; 3 vertices total -> 1 triangle
		glDisableVertexAttribArray(0);

		//End the current frame (internally swaps the front and back buffers)
		window.display();
	}

	//cin.get(); //Waits until the user presses "enter"
	return 1;
}