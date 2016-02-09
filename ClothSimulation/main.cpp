
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
	window = glfwCreateWindow(1500, 950, "Cloth simulator 2016", NULL, NULL);

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
	double aspectRatio = (double)windowHeight / (double)windowWidth; 

	Loader loader;
	Font font{ loader, 0.025, aspectRatio };
	GroundPlane groundPlane{ loader };
	double spherePosZ{ -3 }, spherePosX{ 0 };
	Sphere sphere{ 0, 0.5, -3, 0.5, loader };
	Cloth cloth{ loader, 2, 100}; 
	
	MousePicker mousePicker{ Vec2{ (double)windowWidth ,(double)windowHeight} };
	Sphere mouseDebugSphere{ 0, 0, 0, 0.2, loader };


	vector<Light> allLights;					// a dynamic list of lights

	// one light realy far away (without attenuation)
	allLights.push_back(Light{ 50, 400, 400 });	
	allLights[0].color = Vec3(0.6, 0.6, 0.6);

	// 4 point point lights aranged in a square around the cloth	
	Vec3 frontColor{ 0.8, 0.4, 0.4 };
	Vec3 backColor{ 0.7, 0.7, 0.9 };
	Vec3 attenuation{ 1.0, 0.01, 0.008 };
	double distance = 7; 
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

	// create a new camera object using the current window's aspect ratio 
	Camera camera{ aspectRatio };

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
		
		Vec3 spherePos = sphere.getPos(); 
		Vec3 deltaVector;

		if (UserInput::pollKey(window, GLFW_KEY_UP))
			deltaVector.z = -delta_time * 2;			
		if (UserInput::pollKey(window, GLFW_KEY_DOWN))		
			deltaVector.z = delta_time * 2;

		if (UserInput::pollKey(window, GLFW_KEY_SPACE))
			deltaVector.y = delta_time * 2;
		if (UserInput::pollKey(window, GLFW_KEY_LEFT_SHIFT))
			deltaVector.y = -delta_time * 2;

		if (UserInput::pollKey(window, GLFW_KEY_LEFT))
			deltaVector.x = -delta_time * 2;
		if (UserInput::pollKey(window, GLFW_KEY_RIGHT))
			deltaVector.x = delta_time * 2;

		spherePos += deltaVector; 
		sphere.setPos(spherePos);		

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


		mouseDebugSphere.render(window, camera, allLights);

		// draw the cloth
		cloth.render(window, camera, allLights);
		
		// wireframe rendering is of be default. 
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
		font.setColor(0, 1, 0.5);
		font.render("Frame rate: ", -0.95, 0.92);		
		font.render((int)(1 / delta_time), -0.65, 0.92);

		font.setColor(1, 1, 1);
		font.render("Cloth simulation by Axel Brinkeby, Mikael Lindhe and Eleonora Petersson", -0.95, -0.95);
		
		font.setColor(1, 1, 0);
		font.render("Move the sphere with the arrow keys and Space/L-shift", -0.95, -0.88);

		// =============== mouse picker debug here: =============== 

		font.setColor(1, 1, 1);

		Vec3 ray = mousePicker.calculateMouseRay(camera);
		char tempBuffer[100];
		ostringstream stream;
		stream << fixed << setprecision(2) << "Mouse picker ray: ( " << ray.x << "x, " << ray.y << "y, " << ray.z << "z )";
		strcpy_s(tempBuffer, stream.str().c_str());		
		font.render(tempBuffer, -0.95, -0.81);

		Vec3 cameraPos = mousePicker.getRayStartPoint(camera);
		char tempBuffer2[100];
		ostringstream stream2;
		stream2 << fixed << setprecision(2) << "Camera world pos: ( " << cameraPos.x << "x, " << cameraPos.y << "y, " << cameraPos.z << "z )";
		strcpy_s(tempBuffer2, stream2.str().c_str());
		font.render(tempBuffer2, -0.95, -0.74);

		Vec3 planeIntersection = mousePicker.getPlaneIntersectionPoint(0);
		Vec2 mousePos = UserInput::getMouseNormalizedDeviceCoords((double)windowWidth, (double)windowHeight);
		char tempBuffer3[100];
		ostringstream stream3;
		stream3 << fixed << setprecision(2) << "Ground plane intersection: ( " << planeIntersection.x << "x, " << planeIntersection.y << "y, " << planeIntersection.z << "z )";
		//stream3 << setprecision(2) << "mouse pos: ( " << mousePos.x <<  "x, " << mousePos.y << "y )";
		strcpy_s(tempBuffer3, stream3.str().c_str());
		font.render(tempBuffer3, -0.95, -0.67);

		mouseDebugSphere.setPos(cameraPos + ray);		//

		// =========================================================


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