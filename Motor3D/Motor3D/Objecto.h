
#ifndef OBJECTO_H
#define OBJECTO_H
#include <vector>
#include "Escala.h"
#include "Rotacao.h"
#include "Translacao.h"


class Objecto 
{
private:
	Escala scale;
	Rotacao rotation;
	Translacao translation;
	std::vector<float> coords;
	std::vector<char> transfor;
public:
	void setEscala(float, float, float);
	void setRotacao(float, float, float, float);
	void setTranslacao(float, float, float);
	void setOrder();
	std::vector<float> getCoords();
	float getEscalaX();
	float getEscalaY();
	float getEscalaZ();
	float getRotacaoX();
	float getRotacaoY();
	float getRotacaoZ();
	float getRotacaoAng();
	float getTranslacaoX();
	float getTranslacaoY();
	float getTranslacaoZ();
	std::vector<char> getTransfor();
	void guardaTransfor(std::vector<char> c);
	void guardaCoordenadasOBJ(std::vector<float> x);
};
#endif