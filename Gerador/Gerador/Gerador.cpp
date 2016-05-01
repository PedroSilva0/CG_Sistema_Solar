#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <string>     // std::string, std::stof

using namespace std;


void areaPlana(float altura, float comprimento, char* filename) {
	ofstream file;
	file.open(filename, ios::ate);

	//triangulo 1

	file << -comprimento / 2 << " " << altura / 2 << " " << 0 << "\n";
	file << -comprimento / 2 << " " << -altura / 2 << " " << 0 << "\n";
	file << comprimento / 2 << " " << altura / 2 << " " << 0 << "\n";

	//triângulo 2
	file << comprimento / 2 << " " << altura / 2 << " " << 0 << "\n";
	file << -comprimento / 2 << " " << -altura / 2 << " " << 0 << "\n";
	file << comprimento / 2 << " " << -altura / 2 << " " << 0 << "\n";
	file.close();
}

void paralelipipedo(float altura, float comprimento, float largura, char* filename) {
	ofstream file;
	file.open(filename, ios::ate);
	//face frontal

	//triangulo 1

	file << -comprimento / 2 << " " << altura / 2 << " " << largura / 2 << "\n";
	file << -comprimento / 2 << " " << -altura / 2 << " " << largura / 2 << "\n";
	file << comprimento / 2 << " " << altura / 2 << " " << largura / 2 << "\n";

	//triângulo 2
	file << comprimento / 2 << " " << altura / 2 << " " << largura / 2 << "\n";
	file << -comprimento / 2 << " " << -altura / 2 << " " << largura / 2 << "\n";
	file << comprimento / 2 << " " << -altura / 2 << " " << largura / 2 << "\n";

	//face traseira
	//triangulo 3

	file << -comprimento / 2 << " " << altura / 2 << " " << -largura / 2 << "\n";
	file << comprimento / 2 << " " << altura / 2 << " " << -largura / 2 << "\n";
	file << -comprimento / 2 << " " << -altura / 2 << " " << -largura / 2 << "\n";

	//triângulo 4
	file << comprimento / 2 << " " << altura / 2 << " " << -largura / 2 << "\n";
	file << comprimento / 2 << " " << -altura / 2 << " " << -largura / 2 << "\n";
	file << -comprimento / 2 << " " << -altura / 2 << " " << -largura / 2 << "\n";

	//face superior
	//triangulo 5

	file << -comprimento / 2 << " " << altura / 2 << " " << largura / 2 << "\n";
	file << comprimento / 2 << " " << altura / 2 << " " << largura / 2 << "\n";
	file << comprimento / 2 << " " << altura / 2 << " " << -largura / 2 << "\n";

	//triângulo 6
	file << comprimento / 2 << " " << altura / 2 << " " << -largura / 2 << "\n";
	file << -comprimento / 2 << " " << altura / 2 << " " << -largura / 2 << "\n";
	file << -comprimento / 2 << " " << altura / 2 << " " << largura / 2 << "\n";

	//face inferior
	//triangulo 7

	file << -comprimento / 2 << " " << -altura / 2 << " " << largura / 2 << "\n";
	file << -comprimento / 2 << " " << -altura / 2 << " " << -largura / 2 << "\n";
	file << comprimento / 2 << " " << -altura / 2 << " " << -largura / 2 << "\n";

	//triângulo 8
	file << comprimento / 2 << " " << -altura / 2 << " " << -largura / 2 << "\n";
	file << comprimento / 2 << " " << -altura / 2 << " " << largura / 2 << "\n";
	file << -comprimento / 2 << " " << -altura / 2 << " " << largura / 2 << "\n";

	//face direita
	//triangulo 9

	file << comprimento / 2 << " " << -altura / 2 << " " << largura / 2 << "\n";
	file << comprimento / 2 << " " << -altura / 2 << " " << -largura / 2 << "\n";
	file << comprimento / 2 << " " << altura / 2 << " " << -largura / 2 << "\n";

	//triângulo 10
	file << comprimento / 2 << " " << altura / 2 << " " << -largura / 2 << "\n";
	file << comprimento / 2 << " " << altura / 2 << " " << largura / 2 << "\n";
	file << comprimento / 2 << " " << -altura / 2 << " " << largura / 2 << "\n";

	//face esquerda
	//triangulo 11

	file << -comprimento / 2 << " " << -altura / 2 << " " << largura / 2 << "\n";
	file << -comprimento / 2 << " " << altura / 2 << " " << largura / 2 << "\n";
	file << -comprimento / 2 << " " << altura / 2 << " " << -largura / 2 << "\n";

	//triângulo 12
	file << -comprimento / 2 << " " << altura / 2 << " " << -largura / 2 << "\n";
	file << -comprimento / 2 << " " << -altura / 2 << " " << -largura / 2 << "\n";
	file << -comprimento / 2 << " " << -altura / 2 << " " << largura / 2 << "\n";



	file.close();
}

void cone(float raio, float altura, int fatias, int camadas, char* filename) {
	float salto = altura / camadas;
	int camaux = 1;
	float aux;
	double alpha1 = 0;
	float raioCam = ((altura - salto)*raio) / altura;
	float alturaCam = (-altura / 2) + salto;
	float alturaCam2 = -altura / 2;

	ofstream file;
	file.open(filename, ios::ate);

	//imprimir camadas
	while (camaux <= camadas) {
		//imprimir fatias
		for (aux = 0; aux <= fatias; aux++) {
			file << raioCam * sin(alpha1) << " " << alturaCam << " " << raioCam* cos(alpha1) << "\n";
			file << raio* sin(alpha1) << " " << alturaCam2 << " " << raio* cos(alpha1) << "\n";
			file << raioCam*sin(alpha1 + ((2 * M_PI) / fatias)) << " " << alturaCam << " " << raioCam*cos(alpha1 + ((2 * M_PI) / fatias)) << "\n";
			file << raioCam*sin(alpha1 + ((2 * M_PI) / fatias)) << " " << alturaCam << " " << raioCam*cos(alpha1 + ((2 * M_PI) / fatias)) << "\n";
			file << raio* sin(alpha1) << " " << alturaCam2 << " " << raio* cos(alpha1) << "\n";
			file << raio*sin(alpha1 + ((2 * M_PI) / fatias)) << " " << alturaCam2 << " " << raio*cos(alpha1 + ((2 * M_PI) / fatias)) << "\n";
			alpha1 += (2 * M_PI) / fatias;
		}
		camaux++;
		alturaCam2 = alturaCam;
		alturaCam = alturaCam + salto;
		altura = altura - salto;
		raio = raioCam;
		raioCam = ((altura - salto)*raio) / altura;
	}
	file.close();
}

void esfera(float raio, int fatias, int camadas, char* filename) {
	double raioS, raioI;
	double alpha1, betaS, betaI;
	int auxC, auxF;
	betaS = M_PI / 2;
	betaI = betaS - (M_PI / camadas);
	ofstream file;
	file.open(filename, ios::ate);
	for (auxC = 1; auxC <= camadas; auxC++) {
		raioS = raio*cos(betaS);
		raioI = raio*cos(betaI);
		for (auxF = 0; auxF <= fatias; auxF++) {
			file << raioS*sin(alpha1) << " " << raio*sin(betaS) << " " << raioS*cos(alpha1) << "\n";
			file << raioI*sin(alpha1) << " " << raio*sin(betaI) << " " << raioI*cos(alpha1) << "\n";
			file << raioS*sin(alpha1 + ((2 * M_PI) / fatias)) << " " << raio*sin(betaS) << " " << raioS*cos(alpha1 + ((2 * M_PI) / fatias)) << "\n";
			file << raioS*sin(alpha1 + ((2 * M_PI) / fatias)) << " " << raio*sin(betaS) << " " << raioS*cos(alpha1 + ((2 * M_PI) / fatias)) << "\n";
			file << raioI*sin(alpha1) << " " << raio*sin(betaI) << " " << raioI*cos(alpha1) << "\n";
			file << raioI*sin(alpha1 + ((2 * M_PI) / fatias)) << " " << raio*sin(betaI) << " " << raioI*cos(alpha1 + ((2 * M_PI) / fatias)) << "\n";
			alpha1 += (2 * M_PI) / fatias;
		}
		betaS = betaI;
		betaI = betaI - (M_PI / camadas);
		alpha1 = 0;
	}
	file.close();
}


void anel(float raioFora, float raioDentro, int fatias, char* filename) {

	float x;
	float x_prox;
	float z;
	float z_prox;
	float x_i;
	float x_i_prox;
	float z_i;
	float z_i_prox;
	float alpha = 2 * M_PI / fatias;
	ofstream file;
	file.open(filename, ios::ate);


	if (file.is_open() && file.good())
	{
		for (int j = 0; j < fatias; j++) {

			x = raioFora*sin(alpha*j);
			x_prox = raioFora*sin(alpha*(j + 1));
			z = raioFora*cos(alpha*j);
			z_prox = raioFora*cos(alpha*(j + 1));
			x_i = (raioDentro)*sin(alpha*j);    //isto é o X
			z_i = (raioDentro)*cos(alpha*j);
			x_i_prox = (raioDentro)*sin(alpha*(j + 1)); 
			z_i_prox = (raioDentro)*cos(alpha*(j + 1));

			//face superior
			//primeiro triângulo
			file << x << " " << 0 << " " << z << "\n";
			file << x_prox << " " << 0 << " " << z_prox << "\n";
			file << x_i_prox << " " << 0 << " " << z_i_prox << "\n";
			//segundo triangulo
			file << x << " " << 0 << " " << z << "\n";
			file << x_i_prox << " " << 0 << " " << z_i_prox << "\n";
			file << x_i << " " << 0 << " " << z_i << "\n";

			//face inferior

			//primeiro triangulo
			file << x_i_prox << " " << 0 << " " << z_i_prox << "\n";
			file << x_prox << " " << 0 << " " << z_prox << "\n";
			file << x << " " << 0 << " " << z << "\n";
			//segundo triangulo
			file << x_i << " " << 0 << " " << z_i << "\n";
			file << x_i_prox << " " << 0 << " " << z_i_prox << "\n";
			file << x << " " << 0 << " " << z << "\n";



		}

	}
	file.close();
}


int main(int argc, char **argv) {
	std::string::size_type sz;     // alias of size_t
	//superficie plana
	if ((strcmp(argv[1], "plane") == 0) && argc == 5) {
		areaPlana(stof(argv[2], &sz), stof(argv[3], &sz), argv[4]);
		printf("Ficheiro criado com sucesso.");
		return 0;
	}
	/*else{
		printf("Argumentos insuficientes.");
		return 0;
	}*/

	//paralelipipedo
	if ((strcmp(argv[1], "box") == 0) && argc == 6) {
		paralelipipedo(stof(argv[2], &sz), stof(argv[3], &sz), stof(argv[4], &sz), argv[5]);
		printf("Ficheiro criado com sucesso.");
		return 0;
	}
	/*else{
		printf("Argumentos insuficientes.");
		return 0;
	}*/

	//Cone
	if ((strcmp(argv[1], "cone") == 0) && argc == 7) {
		cone(stof(argv[2], &sz), stof(argv[3], &sz), atoi(argv[4]), atoi(argv[5]), argv[6]);
		printf("Ficheiro criado com sucesso.");
		return 0;
	}
	/*else{
		printf("Argumentos insuficientes.");
		return 0;
	}*/

	//esfera
	if ((strcmp(argv[1], "sphere") == 0) && argc == 6) {
		esfera(stof(argv[2], &sz), atoi(argv[3]), atoi(argv[4]), argv[5]);
		printf("Ficheiro criado com sucesso.");
		return 0;
	}
	/*else{
		printf("Argumentos insuficientes.");
		return 0;
	}*/

	//anel
	if ((strcmp(argv[1], "ring") == 0) && argc == 6) {
		anel(stof(argv[2], &sz), stof(argv[3], &sz), atoi(argv[4]), argv[5]);
		printf("Ficheiro criado com sucesso.");
		return 0;
	}

	//Objecto não identificado
	printf("Primitiva inixestente");
	return 0;
}

