
#ifndef OBJECTO_H
#define OBJECTO_H
#include <vector>
#include "Escala.h"
#include "Rotacao.h"
#include "Translacao.h"
#include "catmull.h"
#include <GL/glew.h>
#include <GL/glut.h>
#pragma comment(lib,"glew32.lib")


class Objecto 
{
private:
	Escala scale;
	Rotacao rotation;
	Translacao translation;
	std::vector<float> coords;
	std::vector<char> transfor;
	GLuint buffer[1];
	catmull cat_trans;

public:
	void setEscala(float, float, float);
	void setRotacao(float, float, float, float);
	void setTranslacao(float, float, float);
	void Objecto::set_cat_trans(std::vector<float> pontos, float tempo);
	std::vector<float> getCoords();
	float getEscalaX();
	float getEscalaY();
	float getEscalaZ();
	float getRotacaoX();
	float getRotacaoY();
	float getRotacaoZ();
	float getRotacaoAng(long);
	float getTranslacaoX();
	float getTranslacaoY();
	float getTranslacaoZ();
	std::vector<char> getTransfor();
	void guardaTransfor(std::vector<char> c);
	void guardaCoordenadasOBJ(std::vector<float> x);
	GLuint getVBOBuffer();
	void setVBOBuffer( );
	void Objecto::get_cat_trans(long current_time, float res[3]);
};
#endif