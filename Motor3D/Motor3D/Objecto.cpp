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

	GLuint Objecto::getVBOBuffer()
	{
		return buffer[0];
	}

	void Objecto::setVBOBuffer()
	{
		GLuint *buffers = NULL;
		//Activar Buffers
		glEnableClientState(GL_VERTEX_ARRAY);
		//Aloca memória para os buffers
		buffers = (GLuint*)malloc(1 * sizeof(GLuint));
		//Aqui dizemos qual é GLuint que vamos usar e quandos buffers tem
		glGenBuffers(1, buffers);
		// Informamos qual vai ser o buffer que vamos usar para guardar a VBO
		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		//Temos 2 campos importantes (2º e 3º), no 2º metemos a memória necessária para guardar todas as coordenadas, e no 3º informamos o array que tem as coordenadas
		glBufferData(GL_ARRAY_BUFFER, coords.size()*sizeof(float), &coords[0], GL_STATIC_DRAW);
		buffer[0] = buffers[0];

	}

	void Objecto::get_cat_trans(long current_time,float res[3])
	{	
		if (cat_trans.get_tempo_anterior() == 0) {
			cat_trans.set_tempo_anterior(current_time);
		}
		else {
			//printf("tempo medio = %f, current_time= %f, tempo_anterior= %f, tempo= %f", cat_trans.get_tempo_medio(), current_time, cat_trans.get_tempo_anterior(), cat_trans.get_tempo());
			float aux = cat_trans.get_tempo_medio() + ((current_time - cat_trans.get_tempo_anterior()) / (cat_trans.get_tempo() * 1000));
			//printf("%f", aux);
			cat_trans.set_tempo_medio(aux);
			cat_trans.set_tempo_anterior(current_time);
			cat_trans.getGlobalCatmullRomPoint(aux, res);
		}
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

	void Objecto::set_cat_trans(std::vector<float> pontos,float tempo)
	{
		catmull c;
		c.set_pontos(pontos);
		c.set_tot_pontos( pontos.size() / 3);
		c.set_tempo(tempo);
		c.set_tempo_medio(0);
		cat_trans = c;
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
	
	float Objecto::getRotacaoAng(long a){
		return rotation.getAng(a);
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