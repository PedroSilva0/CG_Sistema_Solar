#include "Rotacao.h"

void Rotacao::set_values(float ax, float ay, float az, float time){
	tempo = time;
	x = ax;
	y = ay;
	z = az;
	tempo_anterior = 0;
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

float Rotacao::getAng(long current_time){
	float res;
	res = ((current_time - tempo_anterior) * 360) / (tempo * 1000);
	tempo_anterior = current_time;
	return res;
}