
#ifndef OBJECTO_H
#define OBJECTO_H
#include <vector>
#include "Escala.h"
#include "Rotacao.h"
#include "catmull.h"
#include <GL/glew.h>
#include <GL/glut.h>
#pragma comment(lib,"glew32.lib")


class Objecto 
{
private:
	Escala scale;
	Rotacao rotation;
	std::vector<float> coords;
	std::vector<float> normais;
	std::vector<float> texture;
	std::vector<char> transfor;
	GLuint buffer[3];
	catmull cat_trans;
	GLuint texID;

public:
	void setEscala(float, float, float);
	void setRotacao(float, float, float, float);
	void Objecto::set_cat_trans(std::vector<float> pontos, float tempo);
	std::vector<float> getCoords();
	std::vector<float> getNormais();
	std::vector<float> getTexture();
	void loadTexture(std::string s);
	GLuint getTextid();
	float getEscalaX();
	float getEscalaY();
	float getEscalaZ();
	float getRotacaoX();
	float getRotacaoY();
	float getRotacaoZ();
	float getRotacaoAng(long);
	std::vector<char> getTransfor();
	void guardaTransfor(std::vector<char> c);
	void guardaCoordenadasOBJ(std::vector<float> x);
	void setTexture(std::vector<float> x);
	void setNormais(std::vector<float> x);
	GLuint getVBOBuffer();
	void setVBOBuffer( );
	GLuint getNormalVBOBuffer();
	GLuint getTextureVBOBuffer();
	void setNormalVBOBuffer();
	void setTextureVBOBuffer();
	void Objecto::get_cat_trans(long current_time, float res[3]);
};
#endif