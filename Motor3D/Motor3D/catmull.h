#ifndef CATMULL_H
#define CATMULL_H
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#pragma comment(lib,"glew32.lib")


class catmull
{
private:
	float res[3];  //resultado
	std::vector<float> pontos;  //pontos da translação
	int tot_pontos;
	float tempo; //tempo total da translação do planeta em segundos
	float tempo_medio; //(tempo_atual- tempo_anterior)/(tempo*1000)
	float tempo_anterior; //tempo da ultima translação
	

public:
	void set_res(float, float, float);
	void set_pontos(std::vector<float> x);
	void set_tot_pontos(int);
	void set_tempo(float);
	void set_tempo_medio(float);
	void set_tempo_anterior(float);
	void get_res(float r[3]);
	float get_tempo();
	float get_tempo_medio();
	float get_tempo_anterior();
	int get_tot_pontos();
	std::vector<float> get_pontos();
	void getGlobalCatmullRomPoint(float gt, float *res);
	void catmull::getCatmullRomPoint(float t, int *indices, float *res2, std::vector<float> pontos2);
	void catmull::renderCatmullRomCurve();
};
#endif
