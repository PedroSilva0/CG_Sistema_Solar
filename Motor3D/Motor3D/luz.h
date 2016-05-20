#ifndef LUZ_H
#define LUZ_H
#include <vector>
#include "Escala.h"
#include "Rotacao.h"
#include "catmull.h"
#include <GL/glew.h>
#include <GL/glut.h>
#pragma comment(lib,"glew32.lib")


class Luz
{
private:
	float pos[4];
	float amb[3];
	float dif[3];
	int tipo;

public:
	void setPos(float, float, float,float);
	void setAmb(float, float, float);
	void setDiff(float, float, float);
	void getPos(float[4]);
	void getAmb(float[3]);
	void getDif(float[3]);
};
#endif