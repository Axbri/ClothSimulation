#include "ScreenshotTaker.h"

using namespace std;

void ScreenshotTaker::screenshot(int x, int y)
{
	// get the image data
	long imageSize = x * y * 3;
	unsigned char *data = new unsigned char[imageSize];
	glReadPixels(0, 0, x, y, GL_BGR, GL_UNSIGNED_BYTE, data);

	// split x and y sizes into bytes
	int xa = x % 256;
	int xb = (x - xa) / 256;
	int ya = y % 256;
	int yb = (y - ya) / 256;

	// assemble the header
	unsigned char header[18] = { 0,0,2,0,0,0,0,0,0,0,0,0,(char)xa,(char)xb,(char)ya,(char)yb,24,0 };

	// create a filemane containing the current time.
	time_t now = time(0);
	struct tm localtm;
	localtime_s(&localtm, &now);
	char filemanebuffer[100];
	ostringstream stream;
	stream << "screenshot_" << 
		(localtm.tm_year + 1900) << '-' << (localtm.tm_mon + 1) << '-' << localtm.tm_mday << "_" <<
		(localtm.tm_hour) << '.' << (localtm.tm_min) << '.' << localtm.tm_sec << ".bmp";
	strcpy_s(filemanebuffer, stream.str().c_str());

	// write header and data to file
	ofstream file; 
	file.open(filemanebuffer, ios::out | ios::binary);
	file.write(reinterpret_cast <char*>(header), sizeof(char) * 18);
	file.write(reinterpret_cast <char*>(data), sizeof(char)*imageSize);
	file.close();
	
	// delete the image data buffer
	delete[] data;
	data = NULL;
}
