#include "Rotacao.h"

void Rotacao::set_values(float ax, float ay, float az, float aangulo){
	angulo = aangulo;
	x = ax;
	y = ay;
	z = az;
}

float Rotacao::getX(){
	return x;
}

float Rotacao::getY(){
	return y;
}

float Rotacao::getZ(){
	return z;
}

float Rotacao::getAng(){
	return angulo;
}