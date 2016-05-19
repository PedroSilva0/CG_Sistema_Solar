#include "Objecto.h"
#include "Escala.h"
#include "Rotacao.h"
#include "Translacao.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <IL/il.h>
#pragma comment(lib, "devil.lib")


	void Objecto::guardaCoordenadasOBJ(std::vector<float> x){
		coords = x;
	}

	void Objecto::setTexture(std::vector<float> x) {
		texture = x;
	}

	void Objecto::setNormais(std::vector<float> x) {
		normais = x;
	}

	GLuint Objecto::getVBOBuffer()
	{
		return buffer[0];
	}

	GLuint Objecto::getNormalVBOBuffer()
	{
		return buffer[1];
	}

	GLuint Objecto::getTextureVBOBuffer()
	{
		return buffer[2];
	}
	
	GLuint Objecto::getTextid()
	{
		return texID;
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

	void Objecto::setNormalVBOBuffer()
	{
		GLuint *buffers = NULL;
		//Activar Buffers
		glEnableClientState(GL_NORMAL_ARRAY);
		//Aloca memória para os buffers
		buffers = (GLuint*)malloc(1 * sizeof(GLuint));
		//Aqui dizemos qual é GLuint que vamos usar e quandos buffers tem
		glGenBuffers(1, buffers);
		// Informamos qual vai ser o buffer que vamos usar para guardar a VBO
		glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
		//Temos 2 campos importantes (2º e 3º), no 2º metemos a memória necessária para guardar todas as coordenadas, e no 3º informamos o array que tem as coordenadas
		glBufferData(GL_ARRAY_BUFFER, normais.size() * sizeof(float), &normais[0], GL_STATIC_DRAW);
		buffer[1] = buffers[0];

	}

	void Objecto::setTextureVBOBuffer()
	{
		GLuint *buffers = NULL;
		//Activar Buffers
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		//Aloca memória para os buffers
		buffers = (GLuint*)malloc(1 * sizeof(GLuint));
		//Aqui dizemos qual é GLuint que vamos usar e quandos buffers tem
		glGenBuffers(1, buffers);
		// Informamos qual vai ser o buffer que vamos usar para guardar a VBO
		glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
		//Temos 2 campos importantes (2º e 3º), no 2º metemos a memória necessária para guardar todas as coordenadas, e no 3º informamos o array que tem as coordenadas
		glBufferData(GL_ARRAY_BUFFER, texture.size() * sizeof(float), &texture[0], GL_STATIC_DRAW);
		buffer[2] = buffers[0];

	}

	void Objecto::get_cat_trans(long current_time,float res[3])
	{	
		if (cat_trans.get_tempo_anterior() == 0) {
			cat_trans.set_tempo_anterior(current_time);
		}
		else {
			float aux = cat_trans.get_tempo_medio() + ((current_time - cat_trans.get_tempo_anterior()) / (cat_trans.get_tempo() * 1000));
			
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
	
	std::vector<float> Objecto::getTexture() {
		return texture;
	}

	std::vector<float> Objecto::getNormais() {
		return normais;
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

	
	std::vector<char> Objecto::getTransfor(){
		return transfor;
	}

	void Objecto::guardaTransfor(std::vector<char> c){
		transfor = c;
	}

	void Objecto::loadTexture(std::string s) {

		unsigned int t, tw, th;
		unsigned char *texData;
		ilInit();
		ilEnable(IL_ORIGIN_SET);
		ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
		ilGenImages(1, &t);
		ilBindImage(t);
		ilLoadImage((ILstring) s.c_str());
		//ilLoadImage((ILstring)"texture_sol.jpg");
		tw = ilGetInteger(IL_IMAGE_WIDTH);
		th = ilGetInteger(IL_IMAGE_HEIGHT);
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		texData = ilGetData();

		glGenTextures(1, &texID);

		glBindTexture(GL_TEXTURE_2D, texID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);

	}