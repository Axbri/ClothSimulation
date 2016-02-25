
#include <GL/glew.h> 
#include <GLFW/glfw3.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <sstream>
#include <iostream>
#include <iomanip>
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
#include "mousepicker.h"

//Define an error callback  
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

// This function inizializes GLFW and OpenGL and 
// opens a window and returns a pointer to the window. 
GLFWwindow* createWindow()
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
	window = glfwCreateWindow(UserInput::getWindowSize().x, UserInput::getWindowSize().y, 
		"Cloth simulation by Axel Brinkeby, Mikael Lindhe and Eleonora Petersson", NULL, NULL);

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
	glfwSetFramebufferSizeCallback(window, UserInput::framebuffer_size_callback);

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

void addLights(vector<Light> &allLights) {
	// one light realy far away (without attenuation)
	allLights.push_back(Light{ 50, 400, 400 });
	allLights[0].color = Vec3(0.3, 0.3, 0.3);

	// 4 point point lights aranged in a square around the cloth	
	Vec3 frontColor{ 0.7, 0.6, 0.6 };
	Vec3 backColor{ 0.7, 0.6, 0.6 };
	Vec3 attenuation{ 1.0, 0.01, 0.008 };
	double distance = 10;
	double hight = 8;

	allLights.push_back(Light{ distance, hight, distance });
	allLights[1].color = Vec3(frontColor);
	allLights[1].attenuation = Vec3(attenuation);

	allLights.push_back(Light{ -distance, hight, distance });
	allLights[2].color = Vec3(frontColor);
	allLights[2].attenuation = Vec3(attenuation);

	allLights.push_back(Light{ distance, hight, -distance });
	allLights[3].color = Vec3(backColor);
	allLights[3].attenuation = Vec3(attenuation);

	allLights.push_back(Light{ -distance, hight, -distance });
	allLights[4].color = Vec3(backColor);
	allLights[4].attenuation = Vec3(attenuation);

	allLights.push_back(Light{ 0, hight, 0 });
	allLights[5].color = Vec3(frontColor);
	allLights[5].attenuation = Vec3(attenuation);
}

// This is the main function that starts the program. 
int main(void)
{
	GLFWwindow* window = createWindow();			// init GLFW and GLEW

	Loader loader;
	Font font{ loader, 0.025};
	GroundPlane groundPlane{ loader };
	double spherePosZ{ -3 }, spherePosX{ 0 };
	Sphere sphere{ 0, 0.2, 2, 0.2, loader };
	Cloth cloth1{ loader, Vec3{ -0.5, 1.2, 0 }, 1, 100 };	
	//Cloth cloth2{ loader, Vec3{ 0.5, 1.5, 0 }, 2, 100 };
	MousePicker mousePicker{ };
	Camera camera{};

	vector<Light> allLights;	// a dynamic list of lights
	addLights(allLights);		// add all the lights to the list
	

	// set the backgorund color and enable depth testing
	glClearColor(0.4f, 0.6f, 0.7f, 0.0f);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
		
	// variables used in the main loop 
	double previus_time{ 0 }, delta_time{ 0 }, accumulated_time{ 0 };
	int framerate{ 0 }, frames_this_second{ 0 };

	do //Main Loop  
	{ 
		// ================================== update ==================================
		
		Vec3 ray = mousePicker.calculateMouseRay(camera);
		Vec3 cameraPos = mousePicker.calculateStartPoint(camera);
		Vec3 planeIntersection = mousePicker.getPlaneIntersectionPoint(sphere.getRadius());

		// move the sphere with the mouse cursor if the left button is pressed down 
		double distance = Vec3::pointLineDistance(sphere.getPos() - cameraPos, ray);
		if (mousePicker.isPlaneIntersectionValid() && UserInput::getLeftMouseButton())
		{
			if (distance < sphere.getRadius()) 
			{
				sphere.setPos(planeIntersection);
			}			
		}
			
		sphere.updateModelMatrix(delta_time);

		cloth1.update(delta_time, previus_time, sphere);
		//cloth2.update(delta_time, previus_time, sphere);
		camera.update(delta_time);
				
		// ================================== render ==================================
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);

		// draw the groundplane
		groundPlane.render(window, camera, allLights);
		
		// enable wireframe rendering if the user hold down the W key on the keyboard 
		if (UserInput::pollKey(window, GLFW_KEY_W))
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// render the sphere
		sphere.render(window, camera, allLights);
		
		// draw the cloth
		cloth1.render(window, camera, allLights);
		//cloth2.render(window, camera, allLights);
		
		// wireframe rendering is of be default. 
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				
		// render all the text: 
		font.setColor(0, 1, 0.5);
		font.render("Frame rate", framerate, -0.95, 0.92);

		font.setColor(1, 1, 1);
		font.render("Sphere distance", distance, -0.95, 0.86);

		font.setColor(1, 1, 1);
		font.render("Rotate the camera with center mouse button. ", -0.95, -0.81);		
		font.render("Move the sphere by holding it with the left mouse button. ", -0.95, -0.88);
		font.render("Cloth simulation by Axel Brinkeby, Mikael Lindhe and Eleonora Petersson", -0.95, -0.95);
				
		//Swap buffers  
		glfwSwapBuffers(window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...  
		glfwPollEvents();

		// ======================= calculate deltaTime and framerate ========================
		// calculate delta time, used for simulating accurate physics and showing framrate
		delta_time = glfwGetTime() - previus_time;
		previus_time = glfwGetTime();

		accumulated_time += delta_time; 
		frames_this_second++; 
		if (accumulated_time > 1)
		{
			accumulated_time -= 1;
			framerate = frames_this_second; 
			frames_this_second = 0; 
		}
			
	} while (!glfwWindowShouldClose(window));

	cloth1.cleanUp(); 
	//cloth2.cleanUp();
	groundPlane.cleanUp(); 
	loader.cleanUp(); 

	//Close OpenGL window and terminate GLFW  
	glfwDestroyWindow(window);
	//Finalize and clean up GLFW  
	glfwTerminate();

	exit(EXIT_SUCCESS);
}