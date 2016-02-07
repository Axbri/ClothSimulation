
#include <GL/glew.h> 
#include <GLFW/glfw3.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <sstream>
#include <iostream>
#include <vector>

#include "loader.h"
#include "camera.h"
#include "shader.h"
#include "userinput.h"
#include "cloth.h"
#include "groundplane.h"
#include "font.h"
#include "sphere.h"
#include "light.h"
#include "userinput.h"

//Define an error callback  
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

// This function inizializes GLFW and OpenGL and 
// opens a window and returns a pointer to the window. 
GLFWwindow* init()
{
	//Set the error callback  
	glfwSetErrorCallback(error_callback);
	
	//Initialize GLFW  
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional  
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version  
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version  
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing  
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  

	//Declare a window object  
	GLFWwindow* window;
	window = glfwCreateWindow(1200, 800, "Test Window", NULL, NULL);

	//If the window couldn't be created  
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread.   
	glfwMakeContextCurrent(window);

	//Sets callback functions for user input
	glfwSetKeyCallback(window, UserInput::key_callback);
	glfwSetCursorPosCallback(window, UserInput::mouse_pos_callback);
	glfwSetMouseButtonCallback(window, UserInput::mouse_button_callback);
	glfwSetScrollCallback(window, UserInput::scroll_callback);

	//Initialize GLEW   
	glewExperimental = GL_TRUE;
	if (GLenum err = glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}

	// print information about the computer 
	cout << "GLFW version                : " << glfwGetVersionString() << endl;
	cout << "GLEW_VERSION                : " << glewGetString(GLEW_VERSION) << endl;
	cout << "GL_VERSION                  : " << glGetString(GL_VERSION) << endl;
	cout << "GL_VENDOR                   : " << glGetString(GL_VENDOR) << endl;
	cout << "GL_RENDERER                 : " << glGetString(GL_RENDERER) << endl;
	cout << "GL_SHADING_LANGUAGE_VERSION : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl << endl;
	return window;
}

// This is the main function that starts the program. 
int main(void)
{
	GLFWwindow* window = init();			// init GLFW and GLEW

	int windowWidth, windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);
	cout << "window height: " << windowWidth << "px, window width: " << windowHeight << "px " << endl << endl;

	Loader loader;
	Font font{ loader, 0.025 }; 
	GroundPlane groundPlane{ loader };
	double spherePosZ{ -3 }, spherePosX{ 0 };
	Sphere sphere{ 0, 0.5, -3, 0.5, loader };
	Cloth cloth{ loader, 2, 100}; 
	
	vector<Light> allLights;					// a dynamic list of lights

	// one light realy far away (without attenuation)
	allLights.push_back(Light{ 50, 400, 400 });	
	allLights[0].color.set(0.6, 0.6, 0.6);

	// 4 point point lights aranged in a square around the cloth	
	Vec3 frontColor{ 0.8, 0.4, 0.4 };
	Vec3 backColor{ 0.7, 0.7, 0.9 };
	Vec3 attenuation{ 1.0, 0.01, 0.008 };
	double distance = 7; 
	double hight = 8;

	allLights.push_back(Light{ distance, hight, distance });
	allLights[1].color.set(frontColor);
	allLights[1].attenuation.set(attenuation);

	allLights.push_back(Light{ -distance, hight, distance });
	allLights[2].color.set(frontColor);
	allLights[2].attenuation.set(attenuation);

	allLights.push_back(Light{ distance, hight, -distance });
	allLights[3].color.set(backColor);
	allLights[3].attenuation.set(attenuation);

	allLights.push_back(Light{ -distance, hight, -distance });
	allLights[4].color.set(backColor);
	allLights[4].attenuation.set(attenuation);

	// create a new camera object using the current window's aspect ratio 
	Camera camera{ (float)windowHeight / (float)windowWidth };

	// set the backgorund color and enable depth testing
	glClearColor(0.4f, 0.6f, 0.7f, 0.0f);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
		
	// variables used in the main loop 
	double previus_time = 0, delta_time = 0; 

	do //Main Loop  
	{ 
		// ================================== update ==================================
		
		if (UserInput::pollKey(window, GLFW_KEY_UP))
			spherePosZ -= delta_time * 2;
			
		if (UserInput::pollKey(window, GLFW_KEY_DOWN))		
			spherePosZ += delta_time * 2;

		if (UserInput::pollKey(window, GLFW_KEY_LEFT))
			spherePosX -= delta_time * 2;

		if (UserInput::pollKey(window, GLFW_KEY_RIGHT))
			spherePosX += delta_time * 2;

		sphere.setPos(spherePosX, 0.5, spherePosZ);
		
		cloth.update(delta_time, previus_time, sphere);
		camera.update(delta_time);
				
		// ================================== render ==================================
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);

		// draw the groundplane
		groundPlane.render(window, camera, allLights);
		
		// enable wireframe rendering if the user hold down the right mouse button. 
		if (UserInput::getRightMouseButton())
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// render the sphere
		sphere.render(window, camera, allLights);

		// draw the cloth
		cloth.render(window, camera, allLights);
		
		// wireframe rendering is of be default. 
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
		font.render("Frame rate: ", -0.95, 0.92);
		font.setColor(0, 1, 0.5); 
		font.render((int)(1 / delta_time), -0.65, 0.92);

		font.setColor(1, 1, 0);
		font.render("Move the sphere with the arrow keys", -0.95, -0.88);
		
		font.setColor(1, 1, 1);
		font.render("Cloth simulation by Axel Brinkeby, Mikael Lindhe and Eleonora Petersson", -0.95, -0.95);

		//Swap buffers  
		glfwSwapBuffers(window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...  
		glfwPollEvents();

		// ======================= calculate deltaTime and framerate ========================
		// calculate delta time, used for simulating accurate physics and showing framrate
		delta_time = glfwGetTime() - previus_time;
		previus_time = glfwGetTime();

		// show framerate in the title of the window
		ostringstream strs;
		strs << "Framerate: " << (int)(1 / delta_time) << "FPS";
		glfwSetWindowTitle(window, strs.str().c_str());

	} while (!glfwWindowShouldClose(window));

	cloth.cleanUp(); 
	groundPlane.cleanUp(); 
	loader.cleanUp(); 

	//Close OpenGL window and terminate GLFW  
	glfwDestroyWindow(window);
	//Finalize and clean up GLFW  
	glfwTerminate();

	exit(EXIT_SUCCESS);
}