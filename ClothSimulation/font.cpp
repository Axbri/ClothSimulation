#include "font.h"

Font::Font(Loader loader, double size, double aspectRatio)
{
	this->aspectRatio = aspectRatio; 
	this->fontSize = size;
	float positions[8] = { -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, 1.0, -1.0 };
	this->vao = loader.create2Dmodel(positions, 8);
	this->texture = loader.loadBMPtexture("font.bmp");
	fontShader.createShader("fontVert.glsl", "fontFrag.glsl");
	setColor(1, 1, 1); 
}

void Font::setColor(double r, double g, double b)
{
	red = r; 
	green = g; 
	blue = b; 
}

void Font::render(char * name, Vec2 vec, double xPos, double yPos)
{
	char tempBuffer[100];
	ostringstream stream;
	stream << fixed << setprecision(2) << name << ": ( " << vec.x << "x, " << vec.y << "y )";
	strcpy_s(tempBuffer, stream.str().c_str());
	render(tempBuffer, xPos, yPos);
}

void Font::render(char * name, Vec3 vec, double xPos, double yPos)
{
	char tempBuffer[100];
	ostringstream stream;
	stream << fixed << setprecision(2) << name << ": ( " << vec.x << "x, " << vec.y << "y, " << vec.z << "z )";
	strcpy_s(tempBuffer, stream.str().c_str());
	render(tempBuffer, xPos, yPos);
}

void Font::render(char * name, double number, double xPos, double yPos)
{
	char temp[100];
	ostringstream stream;
	stream << name << ": " << number;
	strcpy_s(temp, stream.str().c_str());
	render(temp, xPos, yPos);
}

void Font::render(char * name, int number, double xPos, double yPos)
{
	char temp[100];
	ostringstream stream;
	stream << name << ": " << number;
	strcpy_s(temp, stream.str().c_str());
	render(temp, xPos, yPos);
}

void Font::render(char *str, double xPos, double yPos)
{
	Mat4 pos, scale;
	pos.loadTranslation(xPos, yPos, 0);
	scale.loadScale(fontSize * aspectRatio, fontSize, 0);

	Mat4 transformation = scale * pos; 

	fontShader.start(); 
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	fontShader.setUniformVec4("color", red, green, blue, 1);

	float ofsett = fontSize;
	for (int i = 0; i < strlen(str); i++) {
		if (i != 0) {
			ofsett += getCharWidth(str[i - 1]) * 1.4;
		}

		pos.loadTranslation(xPos + ofsett, yPos, 0);
		transformation = scale * pos;

		fontShader.setUniformMat4("transformationMatrix", transformation.M);
		fontShader.setUniformInt("character", str[i]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);	// 4 vertices in the quad
	}

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	fontShader.stop(); 
}

double Font::getCharWidth(char c) {

	if (c >= '0' && c <= '9') {
		return fontSize * (6.0 / 8.0);
	}

	switch (c) {
	case 'A':
		return fontSize * (6.0 / 8.0);
	case 'B':
		return fontSize * (6.0 / 8.0);
	case 'C':
		return fontSize * (6.0 / 8.0);
	case 'D':
		return fontSize *(6.0 / 8.0);
	case 'E':
		return fontSize * (6.0 / 8.0);
	case 'F':
		return fontSize * (6.0 / 8.0);
	case 'G':
		return fontSize * (6.0 / 8.0);
	case 'H':
		return fontSize * (6.0 / 8.0);
	case 'I':
		return fontSize * (4.0 / 8.0);;
	case 'J':
		return fontSize * (6.0 / 8.0);
	case 'K':
		return fontSize * (6.0 / 8.0);
	case 'L':
		return fontSize * (6.0 / 8.0);
	case 'M':
		return fontSize * (6.0 / 8.0);
	case 'N':
		return fontSize * (6.0 / 8.0);
	case 'O':
		return fontSize * (6.0 / 8.0);
	case 'P':
		return fontSize * (6.0 / 8.0);
	case 'Q':
		return fontSize * (6.0 / 8.0);
	case 'R':
		return fontSize * (6.0 / 8.0);
	case 'S':
		return fontSize * (6.0 / 8.0);
	case 'T':
		return fontSize * (6.0 / 8.0);
	case 'U':
		return fontSize * (6.0 / 8.0);
	case 'V':
		return fontSize * (6.0 / 8.0);
	case 'W':
		return fontSize * (6.0 / 8.0);
	case 'X':
		return fontSize * (6.0 / 8.0);
	case 'Y':
		return fontSize * (6.0 / 8.0);
	case 'Z':
		return fontSize * (6.0 / 8.0);
	case 'Å':
		return fontSize * (6.0 / 8.0);
	case 'Ä':
		return fontSize * (6.0 / 8.0);
	case 'Ö':
		return fontSize * (6.0 / 8.0);

	case 'a':
		return fontSize * (6.0 / 8.0);
	case 'b':
		return fontSize * (6.0 / 8.0);
	case 'c':
		return fontSize * (6.0 / 8.0);
	case 'd':
		return fontSize * (6.0 / 8.0);
	case 'e':
		return fontSize * (6.0 / 8.0);
	case 'f':
		return fontSize * (5.0 / 8.0);
	case 'g':
		return fontSize * (6.0 / 8.0);
	case 'h':
		return fontSize * (6.0 / 8.0);
	case 'i':
		return fontSize * (2.0 / 8.0);
	case 'j':
		return fontSize * (6.0 / 8.0);
	case 'k':
		return fontSize * (5.0 / 8.0);
	case 'l':
		return fontSize * (3.0 / 8.0);
	case 'm':
		return fontSize * (6.0 / 8.0);
	case 'n':
		return fontSize * (6.0 / 8.0);
	case 'o':
		return fontSize * (6.0 / 8.0);
	case 'p':
		return fontSize * (6.0 / 8.0);
	case 'q':
		return fontSize * (6.0 / 8.0);
	case 'r':
		return fontSize * (6.0 / 8.0);
	case 's':
		return fontSize * (6.0 / 8.0);
	case 't':
		return fontSize * (4.0 / 8.0);
	case 'u':
		return fontSize * (6.0 / 8.0);
	case 'v':
		return fontSize * (6.0 / 8.0);
	case 'w':
		return fontSize * (6.0 / 8.0);
	case 'x':
		return fontSize * (6.0 / 8.0);
	case 'y':
		return fontSize * (6.0 / 8.0);
	case 'z':
		return fontSize * (6.0 / 8.0);
	case 'å':
		return fontSize * (6.0 / 8.0);
	case 'ä':
		return fontSize * (6.0 / 8.0);
	case 'ö':
		return fontSize * (6.0 / 8.0);

	case '.':
		return fontSize * (2.0 / 8.0);
	case ',':
		return fontSize * (2.0 / 8.0);
	case ' ':
		return fontSize * (5.0 / 8.0);
	case '_':
		return fontSize * (6.0 / 8.0);
	case '-':
		return fontSize * (6.0 / 8.0);
	case '+':
		return fontSize * (6.0 / 8.0);
	case '/':
		return fontSize * (6.0 / 8.0);
	case '=':
		return fontSize * (6.0 / 8.0);
	case '!':
		return fontSize * (3.0 / 8.0);
	case '"':
		return fontSize * (5.0 / 8.0);

	default:
		return fontSize;
	}

}

void Font::cleanUp()
{
	fontShader.cleanUp(); 
}
