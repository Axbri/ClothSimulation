#pragma once

#include <GLFW/glfw3.h>  
#include "vec3.h"
#include "vec2.h"

class UserInput
{
public:
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods); 
	static void mouse_pos_callback(GLFWwindow* window, double x, double y);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		
	static bool pollKey(GLFWwindow* window, int key);
	static Vec2 getMousePos();
	static Vec2 getMouseNormalizedDeviceCoords(int screenWithPixels, int screenHeightPixels);
	static Vec2 getMouseVel();
	static bool getLeftMouseButton(); 
	static bool getCenterMouseButton();
	static bool getRightMouseButton();
	static double getMouseDeltaScroll();
	
private: 
	static double mouseX; 
	static double mouseY; 
	static double MouseSpeedX;
	static double MouseSpeedY;
	static bool leftMouseButton; 
	static bool centerMouseButton;
	static bool rightMouseButton;
	static double deltaScroll;
};
