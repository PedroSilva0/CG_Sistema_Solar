#include "catmull.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <string>

void catmull::set_res(float x, float y, float z)
{
	res[0] = x;
	res[1] = x;
	res[2] = x;
}

void catmull::set_pontos(std::vector<float> x)
{
	pontos = x;
}

void catmull::set_tot_pontos(int x)
{
	tot_pontos = x;
}

void catmull::set_tempo(float x)
{
	tempo = x;
}

void catmull::set_tempo_medio(float x)
{
	tempo_medio = x;
}

void catmull::set_tempo_anterior(float x)
{
	tempo_anterior = x;
}

void catmull::get_res(float r[3])
{
	r[0] = res[0];
	r[1] = res[1];
	r[2] = res[2];

}

float catmull::get_tempo()
{
	return tempo;
}

float catmull::get_tempo_medio()
{
	return tempo_medio;
}

float catmull::get_tempo_anterior()
{
	return tempo_anterior;
}

int catmull::get_tot_pontos()
{
	return tot_pontos;
}

std::vector<float> catmull::get_pontos()
{
	return pontos;
}



void catmull::getCatmullRomPoint(float t, int *indices, float *res2, std::vector<float> pontos2) {

	float aux[4];
	float matrizT[4] = { t*t*t, t*t, t, 1 };


	// catmull-rom matrix
	float m[4][4] = { { -0.5f,  1.5f, -1.5f,  0.5f },
	{ 1.0f, -2.5f,  2.0f, -0.5f },
	{ -0.5f,  0.0f,  0.5f,  0.0f },
	{ 0.0f,  1.0f,  0.0f,  0.0f } };

	res2[0] = 0.0; res2[1] = 0.0; res2[2] = 0.0;
	// Calcular o ponto res = T * M * P
	// sendo Pi = p[indices[i]]

	for (int i = 0; i < 4; i++) {
		float sumAux = 0;
		for (int j = 0; j < 4; j++) {
			sumAux += matrizT[j] * m[j][i];

		}
		aux[i] = sumAux;
	}
	for (int k = 0; k < 3; k++) {  //3 coordenadas
		float sumAux = 0;
		for (int j = 0; j < 4; j++) {
			sumAux += aux[j] * pontos[indices[j] * 3 + k]; //pontos[j*3+k]

		}
		res2[k] = sumAux;
	}
}

void catmull::getGlobalCatmullRomPoint(float gt, float *res2) {

	float t = gt * tot_pontos; // this is the real global t
	int index = floor(t);  // which segment
	t = t - index; // where within  the segment

				   // indices store the points
	int indices[4];
	indices[0] = (index + tot_pontos - 1) % tot_pontos;
	indices[1] = (indices[0] + 1) % tot_pontos;
	indices[2] = (indices[1] + 1) % tot_pontos;
	indices[3] = (indices[2] + 1) % tot_pontos;

	getCatmullRomPoint(t, indices, res2, pontos);
}

void catmull::renderCatmullRomCurve() {

	// desenhar a curva usando segmentos de reta - GL_LINE_LOOP

	float res2[3];

	glBegin(GL_LINE_LOOP);
	for (float f = 0; f < 1; f += 0.01) {
		getGlobalCatmullRomPoint(f, res2);
		glVertex3f(res2[0], res2[1], res2[2]);
	}
	glEnd();
}

