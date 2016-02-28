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
		
	unsigned char fileheader[14] = {
		'B','M', // magic
		0,0,0,0, // size in bytes
		0,0, // app data
		0,0, // app data
		40 + 14,0,0,0 // start of data offset
	};
	unsigned char infoheader[40] = {
		40,0,0,0, // info hd size
		0,0,0,0, // width
		0,0,0,0, // heigth
		1,0, // number color planes
		24,0, // bits per pixel
		0,0,0,0, // compression is none
		0,0,0,0, // image bits size
		0x13,0x0B,0,0, // horz resoluition in pixel / m
		0x13,0x0B,0,0, // vert resolutions (0x03C3 = 96 dpi, 0x0B13 = 72 dpi)
		0,0,0,0, // #colors in pallete
		0,0,0,0, // #important colors
	};

	infoheader[4] = (unsigned char)(x);
	infoheader[5] = (unsigned char)(x >> 8);
	infoheader[6] = (unsigned char)(x >> 16);
	infoheader[7] = (unsigned char)(x >> 24);

	infoheader[8] = (unsigned char)(y);
	infoheader[9] = (unsigned char)(y >> 8);
	infoheader[10] = (unsigned char)(y >> 16);
	infoheader[11] = (unsigned char)(y >> 24);
	
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
	file.write(reinterpret_cast <char*>(fileheader), sizeof(char) * 14);
	file.write(reinterpret_cast <char*>(infoheader), sizeof(char) * 40);
	file.write(reinterpret_cast <char*>(data), sizeof(char)*imageSize);
	file.close();
	
	// delete the image data buffer
	delete[] data;
	data = NULL;
}
