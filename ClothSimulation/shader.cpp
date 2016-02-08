#include "shader.h"

Shader::Shader()
{

}

// The folowing functions are used to set uniform variables in this shader program. 
// Do not forget to start the shader program before calling any of those functions. 
void Shader::setUniformMat4(char *variableName, float value[])
{
	GLint location = glGetUniformLocation(thisShader, variableName);
	glUniformMatrix4fv(location, 1, GL_TRUE, value);	// 1 = one matrix is loaded to the GPU
}

void Shader::setUniformMat4(char * variableName, Mat4 matrix)
{
	setUniformMat4(variableName, matrix.M); 
}

void Shader::setUniformVec4(char *variableName, float x1, float x2, float x3, float x4)
{
	GLint location = glGetUniformLocation(thisShader, variableName);
	glUniform4f(location, x1, x2, x3, x4);
}

void Shader::setUniformVec4(char * variableName, Vec4 vector)
{
	setUniformVec4(variableName, vector.x1, vector.x2, vector.x3, vector.x4);
}

void Shader::setUniformVec3(char *variableName, float x1, float x2, float x3)
{
	GLint location = glGetUniformLocation(thisShader, variableName);
	glUniform3f(location, x1, x2, x3);
}

void Shader::setUniformVec3(char * variableName, Vec3 vector)
{
	setUniformVec3(variableName, vector.x, vector.y, vector.z);
}

void Shader::setUniformVec2(char *variableName, float x1, float x2)
{
	GLint location = glGetUniformLocation(thisShader, variableName);
	glUniform2f(location, x1, x2);
}

void Shader::setUniformVec2(char * variableName, Vec2 vector)
{
	setUniformVec2(variableName, vector.x, vector.y);
}

void Shader::setUniformFloat(char *variableName, float value)
{
	GLint location = glGetUniformLocation(thisShader, variableName);
	glUniform1f(location, value);
}

void Shader::setUniformInt(char *variableName, int value)
{
	GLint location = glGetUniformLocation(thisShader, variableName);
	glUniform1i(location, value);	
}

// Start this shader program. Call this function before setting uniform variables or rendering stuff. 
void Shader::start()
{
	glUseProgram(thisShader);
}

// Stop using this shader program. 
void Shader::stop()
{
	glUseProgram(0);
}

// Call this function before the program closes to delete the shader program object from memory. 
void Shader::cleanUp()
{
	glDeleteProgram(thisShader);
}

// A private function that reads two textfiles containing the vertex and 
// fragment shader programs, compiles and links them, and creats a shader program object. 
// if any error occur during compilation or linking, they are showd in the console. 
void Shader::createShader(char *vertexshaderfile, char *fragmentshaderfile)
{	
	GLuint vertexShader;
	GLuint fragmentShader;

	const char *vertexShaderStrings[1];
	const char *fragmentShaderStrings[1];
	unsigned char *vertexShaderAssembly;
	unsigned char *fragmentShaderAssembly;

	GLint vertexCompiled;
	GLint fragmentCompiled;
	GLint shadersLinked;
	char str[4096]; // For error messages from the GLSL compiler and linker

					// Create the vertex shader.
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	vertexShaderAssembly = readShaderFile(vertexshaderfile);
	vertexShaderStrings[0] = (char*)vertexShaderAssembly;
	glShaderSource(vertexShader, 1, vertexShaderStrings, NULL);
	glCompileShader(vertexShader);
	free((void *)vertexShaderAssembly);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexCompiled);
	if (vertexCompiled == GL_FALSE)
	{
		glGetShaderInfoLog(vertexShader, sizeof(str), NULL, str);
		fprintf(stderr, "Vertex shader compile error: \n");
		fprintf(stderr, str);
	}
	else
	{
		cout << "Shader program: \"" << vertexshaderfile << "\" compiled sucessfully. " << endl;
	}

	// Create the fragment shader.
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	fragmentShaderAssembly = readShaderFile(fragmentshaderfile);
	fragmentShaderStrings[0] = (char*)fragmentShaderAssembly;
	glShaderSource(fragmentShader, 1, fragmentShaderStrings, NULL);
	glCompileShader(fragmentShader);
	free((void *)fragmentShaderAssembly);

	glGetProgramiv(fragmentShader, GL_COMPILE_STATUS, &fragmentCompiled);
	if (fragmentCompiled == GL_FALSE)
	{
		glGetShaderInfoLog(fragmentShader, sizeof(str), NULL, str);
		fprintf(stderr, "Fragment shader compile error: \n");
		fprintf(stderr, str);
	}
	else
	{
		cout << "Shader program: \"" << fragmentshaderfile << "\" compiled sucessfully. " << endl;
	}

	// Create a program object and attach the two compiled shaders.
	thisShader = glCreateProgram();
	glAttachShader(thisShader, vertexShader);
	glAttachShader(thisShader, fragmentShader);

	// Link the program object and print out the info log.
	glLinkProgram(thisShader);
	glGetProgramiv(thisShader, GL_LINK_STATUS, &shadersLinked);

	if (shadersLinked == GL_FALSE)
	{
		glGetProgramInfoLog(thisShader, sizeof(str), NULL, str);
		fprintf(stderr, "Program object linking error: \n");
		fprintf(stderr, str);
	} 
	else
	{
		cout << "Shader program: \"" << vertexshaderfile << " / " << fragmentshaderfile << "\" linked sucessfully. " << endl << endl;
	}

	glDeleteShader(vertexShader);   // These are no longer needed
	glDeleteShader(fragmentShader); // after successful linking	
}

// Override the Win32 filelength() function with a version that takes a 
// Unix-style file handle as input instead of a file ID number, 
// and which works on platforms other than Windows.
long Shader::filelength(FILE *file) {
	long numbytes;
	long savedpos = ftell(file); // Remember where we are
	fseek(file, 0, SEEK_END);    // Fast forward to the end
	numbytes = ftell(file);      // Index of last byte in file
	fseek(file, savedpos, SEEK_SET); // Get back to where we were
	return numbytes;             // This is the file length
}


// ReadShaderFile(filename) - read a shader source string from a text-file
unsigned char* Shader::readShaderFile(const char *filename) {
	FILE *file;
	fopen_s(&file, filename, "r");

	if (file == NULL)
	{
		fprintf(stderr, "Cannot open shader file!");
		return 0;
	}
	int bytesinfile = filelength(file);
	unsigned char *buffer = (unsigned char*)malloc(bytesinfile + 1);
	int bytesread = fread(buffer, 1, bytesinfile, file);
	buffer[bytesread] = 0; // Terminate the string with 0
	fclose(file);
	return buffer;
}

