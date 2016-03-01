#pragma once

#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <fstream>
#include <time.h>
#include <ctime>
#include <iomanip>
#include <sstream>

class ScreenshotTaker
{
public:
	// x and y is the size of the frambuffer/image to save
	void screenshot(int x, int y);
};

