#include "Translacao.h"


void Translacao::set_values(float ax, float ay, float az){
	x = ax;
	y = ay;
	z = az;
}

float Translacao::getX(){
	return x;
}

float Translacao::getY(){
	return y;
}

float Translacao::getZ(){
	return z;
}