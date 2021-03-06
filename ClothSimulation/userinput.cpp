#include "userinput.h"

double UserInput::mouseX = 0.0;
double UserInput::mouseY = 0.0;
double UserInput::MouseSpeedX = 0.0;
double UserInput::MouseSpeedY = 0.0;
bool UserInput::leftMouseButton = false;
bool UserInput::centerMouseButton = false;
bool UserInput::rightMouseButton = false;
double UserInput::deltaScroll = 0.0;
Vec2 UserInput::windowSizePixels = Vec2{ 1200, 800 }; 

// Callback function for keyboard input from the user. 
// This function is called automaticaly every time a key on the keybord is pressed or relesed. 
void UserInput::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// close the program if the user presses the escape-key
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) 
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}		
}

// Callback function for mouse position input from the user. 
// This function is called automaticaly every time the mouse cursor is moved. 
void UserInput::mouse_pos_callback(GLFWwindow* window, double x, double y)
{
	double previusMouseX = mouseX;
	double previusMouseY = mouseY;
	mouseX = x;
	mouseY = y;
	MouseSpeedX = mouseX - previusMouseX; 
	MouseSpeedY = mouseY - previusMouseY;
}

// Callback function for mouse button input from the user.
// This function is calledautomaticaly every time a mouse button is pressed or relesed. 
void UserInput::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	
	if (button == GLFW_MOUSE_BUTTON_1) 
	{
		if (action == GLFW_PRESS) 
		{
			leftMouseButton = true;
		}
		else 
		{
			leftMouseButton = false;
		}
	}

	if (button == GLFW_MOUSE_BUTTON_2) 
	{
		if (action == GLFW_PRESS) 
		{
			rightMouseButton = true;
		}
		else 
		{
			rightMouseButton = false;
		}
	}

	if (button == GLFW_MOUSE_BUTTON_3) 
	{
		if (action == GLFW_PRESS) 
		{
			centerMouseButton = true;
		}
		else 
		{
			centerMouseButton = false;
		}
	}
}

// Callback function for the scroll wheel on the mouse. 
// This function is called automaticaly every time the scorll wheel on the mouse is moved. 
void UserInput::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	deltaScroll = yoffset;
}

// this is a callback function that is automaticaly called every time the size of the frambuffer 
// (the part of the screen the openGL renders to) changes. This function saves the new size 
// of the framebuffer to a variable and adjusts the openGL Viewport. 
void UserInput::framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	windowSizePixels = Vec2(width, height);
	glViewport(0, 0, width, height);
}

// check if a key on the keyborad is pressed or not. Returns true if it is, otherwise false. 
bool UserInput::pollKey(GLFWwindow* window, int key)
{
	int state = glfwGetKey(window, key);
	if (state == GLFW_PRESS)
		return true; 
	else
		return false;
}

// Get the absolute pixel position of the mouse curser in the window. 
// The possition is returnet in pixel coordinates
Vec2 UserInput::getMousePos()
{
	return Vec2(mouseX, mouseY); 
}

// Get the absolute pixel position of the mouse curser in the window. The position is returned in 
// normalized device coordinates. coords ranging from -1 to 1. 
Vec2 UserInput::getMouseNormalizedDeviceCoords()
{
	double normalizedX = ((mouseX / windowSizePixels.x) * 2) - 1;
	double normalizedY = ((mouseY / windowSizePixels.y) * 2) - 1;
	return Vec2(normalizedX, -normalizedY);
}

// get the velosity of the mouse curser. How mush the curser has moved sice 
// the last time this function was called or the last time the mouse moved. 
Vec2 UserInput::getMouseVel()
{
	double dx = MouseSpeedX;
	double dy = MouseSpeedY;
	MouseSpeedX = 0; 
	MouseSpeedY = 0;
	return Vec2(dx, dy); 
}

// Get the boolean state the the left mouse button, 
// true if it is pressed, false otherwise. 
bool UserInput::getLeftMouseButton()
{
	return leftMouseButton;
}

// Get the boolean state the the center mouse button 
// (press on the croll wheel), true if it is pressed, false otherwise. 
bool UserInput::getCenterMouseButton()
{
	return centerMouseButton;
}

// Get the boolean state the the left mouse button, 
// true if it is pressed, false otherwise. 
bool UserInput::getRightMouseButton()
{
	return rightMouseButton;
}

// the how hush the scroll whell hase moved 
// since last time this function vas called. 
double UserInput::getMouseDeltaScroll()
{
	double temp = deltaScroll; 
	deltaScroll = 0; 
	return temp;
}

Vec2 UserInput::getWindowSize()
{
	return Vec2{ windowSizePixels };
}