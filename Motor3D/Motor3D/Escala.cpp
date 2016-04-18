#include "Escala.h"


Escala::Escala(){
	x = 0;
	y = 0;
	z = 0;
}

void Escala::set_values(float ax, float ay, float az){
	x = ax;
	y = ay;
	z = az;
}

float Escala::getX(){
	return x;
}

float Escala::getY(){
	return y;
}

float Escala::getZ(){
	return z;
}