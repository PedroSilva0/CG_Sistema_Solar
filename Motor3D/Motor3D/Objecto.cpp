#include "Objecto.h"
#include "Escala.h"
#include "Rotacao.h"
#include "Translacao.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <string>


	void Objecto::guardaCoordenadasOBJ(std::vector<float> x){
		coords = x;
	}

	void Objecto::setEscala(float ax,float ay,float az){
		scale.set_values(ax, ay, az);
	}

	void Objecto::setRotacao(float ax, float ay, float az, float angulo){
		rotation.set_values(ax, ay, az,angulo);
	}

	void Objecto::setTranslacao(float ax, float ay, float az){
		translation.set_values(ax, ay, az);
	}


	std::vector<float> Objecto::getCoords(){
		return coords;
	}

	float Objecto::getEscalaX(){
		return scale.getX();
	}

	float Objecto::getEscalaY(){
		return scale.getY();
	}

	float Objecto::getEscalaZ(){
		return scale.getZ();
	}
	
	float Objecto::getRotacaoX(){
		return rotation.getX();
	}
	
	float Objecto::getRotacaoY(){
		return rotation.getY();
	}
	
	float Objecto::getRotacaoZ(){
		return rotation.getZ();
	}
	
	float Objecto::getRotacaoAng(){
		return rotation.getAng();
	}

	float Objecto::getTranslacaoX(){
		return translation.getX();
	}

	float Objecto::getTranslacaoY(){
		return translation.getY();
	}

	float Objecto::getTranslacaoZ(){
		return translation.getZ();
	}

	std::vector<char> Objecto::getTransfor(){
		return transfor;
	}

	void Objecto::guardaTransfor(std::vector<char> c){
		transfor = c;
	}