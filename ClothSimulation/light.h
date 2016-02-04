#pragma once

class Light
{
public:
	Light();
	Light(double xPos, double yPos, double zPos);
	~Light();
	double get_xPos(); 
	double get_yPos();
	double get_zPos();

private: 
	double xPos, yPos, zPos; 
};

