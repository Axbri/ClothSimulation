#ifndef USERINPUT_H_INCLUDED
#define USERINPUT_H_INCLUDED

#include <GLFW/glfw3.h>  

class UserInput
{
public:
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods); 
	static void mouse_pos_callback(GLFWwindow* window, double x, double y);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	
	static void getMousePos(double &x, double &y);
	static void getMouseVel(double &dx, double &dy);
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


#endif
