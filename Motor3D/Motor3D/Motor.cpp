#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include "tinyxml.h"
#include "tinystr.h"
#include "Objecto.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstring>
#pragma comment(lib,"glew32.lib")

//using namespace std;

float px = 0, py = 0, pz = 0;
float px_c = 0, py_c = 0, pz_c = 0;
float rt_tempo = 0;
float px2 = 0, py2 = 0, pz2 = 0;
float eixoX = 0, eixoY = 0, eixoZ = 0, angulo = 0.0f;
float sX = 1, sY = 1, sZ = 1;
float angulo2 = 0, angulo3 = 0;
std::vector<Objecto> objectos;
std::vector<float> coords;
std::vector<char> transfor;
//catmull
float tr_tempo=0;
std::vector<float> catmull_pontos;
float raio = 100, cam_h = 0.5, cam_v = 0.3, camh_x = 0, camh_y = 0;
//Para ver o numero de FPS
int times, timebase, frame = 0, fps = 0;
char print[20] = "";

//Sem VBOS 900

//VBO
GLuint buffer[1];



void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void desenharVBO() {
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, coords.size() / 3);
}

/*Guarda as coordenadas em vetor dinâmico*/
void guardaCoordenadas(const char* filename) {
	int linhas = 0;
	std::string line;
	std::ifstream infile(filename);
	if (infile.is_open()) {
		while (getline(infile, line)) {
			linhas++;
			char * cstr = new char[line.length() + 1];
			strcpy(cstr, line.c_str());
			char * p = strtok(cstr, " ");
			while (p != 0) {
				coords.push_back(atof(p));
				p = strtok(NULL, " ");
			}
		}
	}
	else {
		printf("Não abri ficheiro\n");
	}
}



/*Guarda objecto*/
void guardaObjecto() {
	Objecto obj;
	obj.guardaCoordenadasOBJ(coords);
	obj.set_cat_trans(catmull_pontos, tr_tempo);
	tr_tempo = 0;
	catmull_pontos.clear();
	obj.setVBOBuffer();
	//coords.clear();
	obj.setEscala(sX, sY, sZ);
	sX = 1; sY = 1; sZ = 1;
	obj.setRotacao(eixoX, eixoY, eixoZ, rt_tempo);
	eixoX = 0; eixoY = 0; eixoZ = 0; rt_tempo = 0;
	obj.setTranslacao(px, py, pz);
	px = 0; py = 0; pz = 0;
	obj.guardaTransfor(transfor);
	transfor.clear();
	coords.clear();
	objectos.push_back(obj);
}

void drawScreen() {
	glBegin(GL_TRIANGLES);
	glColor3f(1, 0, 0);
	for (size_t i = 0; i < coords.size(); i = i + 3) {
		glVertex3f(coords[i], coords[i + 1], coords[i + 2]);
	}
	glEnd();
}

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	
	glLoadIdentity();
	gluLookAt(raio*sin(cam_h)*cos(cam_v), raio*sin(cam_v), raio*cos(cam_h)*cos(cam_v),
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	glTranslatef(px2, py2, pz2);
	glRotatef(angulo3, 0, 1, 0);
	glRotatef(angulo2, 1, 0, 0);
	glRotatef(180, 0, 0, 1);

	Objecto aux;
	glPushMatrix();
	//GRUPO GERAL - TRANSFORMAÇÕES E COORDENADAS
	aux = objectos[0];
	transfor = aux.getTransfor();
	//glRotatef(rt_tempo, 0, 1, 0);
	//rt_tempo += 10;
	for (size_t j = 0; j < transfor.size(); j++) {
		if (transfor[j] == 't') {
			glTranslatef(aux.getTranslacaoX(), aux.getTranslacaoY(), aux.getTranslacaoZ());
		}
		if (transfor[j] == 'r') {
			glRotatef(aux.getRotacaoAng(glutGet(GLUT_ELAPSED_TIME)), aux.getRotacaoX(), aux.getRotacaoY(), aux.getRotacaoZ());
		}
		if (transfor[j] == 's') {
			glScalef(aux.getEscalaX(), aux.getEscalaY(), aux.getEscalaZ());
		}
	}
	coords = aux.getCoords();
	buffer[0] = aux.getVBOBuffer();
	//printf("%d\n", buffer[1]);
	//drawScreen();
	desenharVBO();
	glPopMatrix();


	// RESTANTES GRUPOS

	for (size_t i = 1; i < objectos.size(); i++) {
		aux = objectos[i];
		
		glPushMatrix();
		//Posição da terra
		if (i == 4) {
			Objecto terra = objectos[3];
			float cat_trans[3];
			terra.get_cat_trans(glutGet(GLUT_ELAPSED_TIME), cat_trans);
			glTranslatef(cat_trans[0], cat_trans[1], cat_trans[2]);

		}
		transfor = aux.getTransfor();
		for (size_t j = 0; j < transfor.size(); j++) {
			if (transfor[j] == 't') {
				//glTranslatef(aux.getTranslacaoX(), aux.getTranslacaoY(), aux.getTranslacaoZ());
				float cat_trans[3];
				aux.get_cat_trans(glutGet(GLUT_ELAPSED_TIME), cat_trans);
				glTranslatef(cat_trans[0], cat_trans[1], cat_trans[2]);
			}
			if (transfor[j] == 'r') {
				glRotatef(aux.getRotacaoAng(glutGet(GLUT_ELAPSED_TIME)), aux.getRotacaoX(), aux.getRotacaoY(), aux.getRotacaoZ());
			}
			if (transfor[j] == 's') {
				glScalef(aux.getEscalaX(), aux.getEscalaY(), aux.getEscalaZ());
			}
		}

		//glPopMatrix();
		//glPushMatrix();
		



		coords = aux.getCoords();
		//drawScreen();
		buffer[0] = aux.getVBOBuffer();
		desenharVBO();
		glPopMatrix();

	}

	//Medir FPS (Só dá para ver direito de desligarmos o V-Sync)
	frame++;
	times = glutGet(GLUT_ELAPSED_TIME);
	if (times - timebase > 1000) {
		fps = frame*1000.0 / (times - timebase);
		timebase = times;
		frame = 0;
	}
	sprintf(print, "%d", fps);
	glutSetWindowTitle(print);

	// End of frame
	glutSwapBuffers();
}

// escrever função de processamento do teclado


void setas(int tecla, int x, int y) {

	switch (tecla) {
	case GLUT_KEY_UP:
		if (cam_v + 0.05 < M_PI_2)   //Para câmera não virar ao contrário
			cam_v += 0.05;
		break;
	case GLUT_KEY_DOWN:
		if (cam_v - 0.05 > -M_PI_2)  //Para câmera não virar ao contrário
			cam_v -= 0.05;
		break;

	case GLUT_KEY_LEFT:
		cam_h -= 0.05;
		break;
	case GLUT_KEY_RIGHT:
		cam_h += 0.05;
		break;

	default:
		break;
	}
	glutPostRedisplay();
}

void teclado(unsigned char tecla, int x, int y) {
	switch (tecla) {
	case 'q':
		raio -= 1;
		break;
	case 'e':
		raio += 1;
		break;
	case 'd':px2 = px2 + 1; break;
	case 'a':px2 = px2 - 1; break;
	case 'w':py2 = py2 + 1; break;
	case 's':py2 = py2 - 1; break;
	case 'j': {angulo3 = angulo3 - 1; } break; //roda para a esquerda
	case 'l': {angulo3 = angulo3 + 1; } break; //roda para a direira
	case 'i': {angulo2 = angulo2 + 1; } break; //roda para baixo
	case 'k': {angulo2 = angulo2 - 1; } break; //roda para cima
	default:
		break;
	}
	glutPostRedisplay();
}


// escrever função de processamento do menu

void menuop(int id_op) {
	switch (id_op) {
	case 1:glPolygonMode(GL_FRONT, GL_FILL); break;
	case 2:glPolygonMode(GL_FRONT, GL_LINE); break;
	case 3:glPolygonMode(GL_FRONT, GL_POINT); break;
	case 4:exit(0); break;
	}
	glutPostRedisplay();
}



void handleGrupo(TiXmlNode* pNode) {
	if (pNode != NULL) {
		TiXmlElement* name = pNode->ToElement();
		if (!strcmp(name->Value(), "group")) {
			handleGrupo(pNode->FirstChild());
			handleGrupo(pNode->NextSibling());
		}
		/*if (!strcmp(name->Value(), "translate")) {
			name->QueryFloatAttribute("X", &px);
			name->QueryFloatAttribute("Y", &py);
			name->QueryFloatAttribute("Z", &pz);
			transfor.push_back('t');
			handleGrupo(pNode->NextSibling());
		}*/
		/*if (!strcmp(name->Value(), "rotate")) {
			name->QueryFloatAttribute("angle", &angulo);
			name->QueryFloatAttribute("axisX", &eixoX);
			name->QueryFloatAttribute("axisY", &eixoY);
			name->QueryFloatAttribute("axisZ", &eixoZ);
			transfor.push_back('r');
			handleGrupo(pNode->NextSibling());
		}*/
		if (!strcmp(name->Value(), "rotate")) {
			name->QueryFloatAttribute("time", &rt_tempo);
			name->QueryFloatAttribute("axisX", &eixoX);
			name->QueryFloatAttribute("axisY", &eixoY);
			name->QueryFloatAttribute("axisZ", &eixoZ);
			transfor.push_back('r');
			handleGrupo(pNode->NextSibling());
		}
		if (!strcmp(name->Value(), "scale")) {
			name->QueryFloatAttribute("X", &sX);
			name->QueryFloatAttribute("Y", &sY);
			name->QueryFloatAttribute("Z", &sZ);
			transfor.push_back('s');
			handleGrupo(pNode->NextSibling());
		}
		if (!strcmp(name->Value(), "translate")) {
			name->QueryFloatAttribute("time", &tr_tempo);
			transfor.push_back('t');
			handleGrupo(pNode->FirstChild());
			handleGrupo(pNode->NextSibling());
		}
		if (!strcmp(name->Value(), "point")) {
			name->QueryFloatAttribute("X", &px_c);
			name->QueryFloatAttribute("Y", &py_c);
			name->QueryFloatAttribute("Z", &pz_c);
			catmull_pontos.push_back(px_c);
			catmull_pontos.push_back(py_c);
			catmull_pontos.push_back(pz_c);
			handleGrupo(pNode->NextSibling());
		}
		if (!strcmp(name->Value(), "models")) {
			handleGrupo(pNode->FirstChild());
			handleGrupo(pNode->NextSibling());
		}
		if (!strcmp(name->Value(), "model")) {
			TiXmlElement* filename = pNode->ToElement();
			while (filename != NULL) {
				std::string str = filename->Attribute("file");
				guardaCoordenadas(str.c_str());
				filename = filename->NextSiblingElement();
			}
			guardaObjecto();
		}
	}
}

int main(int argc, char **argv) {
	int menu1;
	// inicialização
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("CG@DI-UM");


	// registo de funções 
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

	// pôr aqui registo da funções do teclado e rato
	glutSpecialFunc(setas);
	glutKeyboardFunc(teclado);

	// pôr aqui a criação do menu
	menu1 = glutCreateMenu(menuop);
	glutAddMenuEntry("Preenchido", 1);
	glutAddMenuEntry("Linhas", 2);
	glutAddMenuEntry("Pontos", 3);
	glutAddMenuEntry("Exit", 4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// alguns settings para OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	//Iniciar o glew 
	glewInit();


	
	TiXmlDocument doc;
	TiXmlDocument docAux;
	if (argc == 2) {
		TiXmlDocument doc(argv[1]);
		doc.LoadFile();
		if (doc.Error()) {
			printf("Error in %s: %s\n", doc.Value(), doc.ErrorDesc());
			exit(1);
		}
		TiXmlNode* node = 0;
		handleGrupo(node->FirstChild());
	}
	else {
		TiXmlDocument doc("sistema_solar.xml");
		//TiXmlDocument doc("teste.xml");
		doc.LoadFile();
		if (doc.Error()) {
			printf("Error in %s: %s\n", doc.Value(), doc.ErrorDesc());
			exit(1);
		}
		TiXmlNode* node = 0;
		node = doc.RootElement();
		handleGrupo(node->FirstChild());
	}

	// entrar no ciclo do GLUT 
	glutMainLoop();

	return 1;
}
