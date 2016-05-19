#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <IL/il.h>
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
#pragma comment(lib, "devil.lib")

//using namespace std;


float px_c = 0, py_c = 0, pz_c = 0;
float rt_tempo = 0;
float px2 = 0, py2 = 0, pz2 = 0;
float eixoX = 0, eixoY = 0, eixoZ = 0;
float sX = 1, sY = 1, sZ = 1;
float angulo2 = 0, angulo3 = 0;
std::vector<Objecto> objectos;
std::vector<float> coords;
std::vector<float> normais;
std::vector<float> textura;
std::vector<char> transfor;
std::string texture_filename;

//catmull
float tr_tempo=0;
std::vector<float> catmull_pontos;
float raio = 100, cam_h = 0.5, cam_v = 0.3, camh_x = 0, camh_y = 0;
//Para ver o numero de FPS
int times, timebase, frame = 0, fps = 0;
char print[20] = "";

//Sem VBOS 900

//VBO
GLuint buffer[3];  // 0 é coordenadas, 1 é normais,2 e textura



//LUZ
float pos[4] = { -1,0,0,1 },
amb[3] = { 1,1,1 },
diff[3] = { 1,1,1 },
matt[4] = { 1,1,1,1 };

//devil
GLuint texID;


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
	//VBO de coordenadas
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	//VBO de normais
	glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
	glNormalPointer(GL_FLOAT,0,0);

	//VBO de texturas
	glBindBuffer(GL_ARRAY_BUFFER, buffer[2]);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);
	glBindTexture(GL_TEXTURE_2D, texID);

	//float white[4] = { 0.7,0.3,0.6,0.5 };
	//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);

	glDrawArrays(GL_TRIANGLES, 0, coords.size() / 3);

	glBindTexture(GL_TEXTURE_2D, 0);

}

/*Guarda as coordenadas em vetor dinâmico*/
void guardaCoordenadas(const char* filename) {
	int linhas = 0;
	std::string line;
	std::ifstream infile(filename);
	if (infile.is_open()) {
		while (getline(infile, line)) {
			if (linhas % 3 == 0) {
				linhas++;
				char * cstr = new char[line.length() + 1];
				strcpy(cstr, line.c_str());
				char * p = strtok(cstr, " ");
				while (p != 0) {
					coords.push_back(atof(p));
					p = strtok(NULL, " ");
				}
			}
			else if (linhas % 3 == 1) {
				linhas++;
				char * cstr = new char[line.length() + 1];
				strcpy(cstr, line.c_str());
				char * p = strtok(cstr, " ");
				while (p != 0) {
					normais.push_back(atof(p));
					p = strtok(NULL, " ");
				}
			}
			else {
				linhas++;
				char * cstr = new char[line.length() + 1];
				strcpy(cstr, line.c_str());
				char * p = strtok(cstr, " ");
				while (p != 0) {
					textura.push_back(atof(p));
					p = strtok(NULL, " ");
				}
			}
		}
	}
	else {
		printf("Não abri ficheiro\n");
	}
	//printf("tot_linhas: %d\n",linhas );
}



/*Guarda objecto*/
void guardaObjecto() {
	Objecto obj;
	obj.loadTexture(texture_filename);
	obj.guardaCoordenadasOBJ(coords);  //guarda coordenadas dos pontos
	obj.setNormais(normais);			// guarda normais dos pontos
	obj.setTexture(textura);			//guarda coords de textura
	obj.set_cat_trans(catmull_pontos, tr_tempo);  //guarda os pontos a serem utilizados na translação e o tempo
	tr_tempo = 0;						//reset do tempo
	catmull_pontos.clear();				//reset dos pontos
	obj.setVBOBuffer();					//guarda o buffer a ser utilizado no desenho dos VBO (coordenadas)
	obj.setNormalVBOBuffer();           // guarda o buffer a ser utilizado no desenho dos VBO (normais)
	obj.setTextureVBOBuffer();			//guarda o buffer a ser utilizado no desenho dos VBO (textura)
	obj.setEscala(sX, sY, sZ);			//guarda a escala do objecto
	sX = 1; sY = 1; sZ = 1;				//reset da escala
	obj.setRotacao(eixoX, eixoY, eixoZ, rt_tempo);	//guarda a rotação do objecto
	eixoX = 0; eixoY = 0; eixoZ = 0; rt_tempo = 0;	//reset da rotação global
	obj.guardaTransfor(transfor);					// guarda ordem das transformações
	transfor.clear();							//reset da ordem de transformações
	coords.clear();								//reset das coordenadas
	normais.clear();							//reset das normais
	textura.clear();							//reset das texturas
	objectos.push_back(obj);					//guarda objecto
}

void drawScreen() {
	glBegin(GL_TRIANGLES);
	//glColor3f(1, 0, 0);
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
	//glTranslatef(4, 0, 0);
	glRotatef(angulo3, 0, 1, 0);
	glRotatef(angulo2, 1, 0, 0);
	glRotatef(180, 0, 0, 1);
		float pos1[4] = {0,20,0,1};
	 float amb1[3] = {1,1,1};
	 float dif1[3] = {0,0,0};
	 float esp1[3] = { 0.3,0.3,0.3 };
		glEnable(GL_LIGHT1);
		glLightfv(GL_LIGHT1, GL_POSITION, pos1);
		glLightfv(GL_LIGHT1, GL_AMBIENT, amb1);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, dif1);

		float pos2[4] = { 0,-17,0,1 };
		float amb2[3] = { 1,1,1 };
		float dif2[3] = { 0,0,0 };
		float esp2[3] = { 0.3,0.3,0.3 };
		glEnable(GL_LIGHT2);
		glLightfv(GL_LIGHT2, GL_POSITION, pos2);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, dif2);
		
		float pos3[4] = { 20,0,0,1 };
		float amb3[3] = { 1,1,1 };
		float dif3[3] = { 1,1,1 };
		float esp3[3] = { 0.3,0.3,0.3 };
		glEnable(GL_LIGHT3);
		glLightfv(GL_LIGHT3, GL_POSITION, pos3);
		glLightfv(GL_LIGHT3, GL_DIFFUSE, dif3);

		float pos4[4] = { -17,0,0,1 };
		float amb4[3] = { 1,1,1 };
		float dif4[3] = { 1,1,1 };
		float esp4[3] = { 0.3,0.3,0.3 };
		glEnable(GL_LIGHT4);
		glLightfv(GL_LIGHT4, GL_POSITION, pos4);
		glLightfv(GL_LIGHT4, GL_DIFFUSE, dif4);

		float pos5[4] = { 0,0,17,1 };
		float amb5[3] = { 1,1,1 };
		float dif5[3] = { 1,1,1 };
		float esp5[3] = { 0.3,0.3,0.3 };
		glEnable(GL_LIGHT5);
		glLightfv(GL_LIGHT5, GL_POSITION, pos5);
		glLightfv(GL_LIGHT5, GL_DIFFUSE, dif5);

		float pos6[4] = { 0,0,17,1 };
		float amb6[3] = { 0,0,0 };
		float dif6[3] = { 0,0,0 };
		float esp6[3] = { 0.3,0.3,0.3 };
		glEnable(GL_LIGHT6);
		glLightfv(GL_LIGHT6, GL_POSITION, pos6);
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb6);
		glLightfv(GL_LIGHT6, GL_DIFFUSE, dif6);
		
		
		

	/*glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);*/
	//glShadeModel(GL_SMOOTH);
	//glLightfv(GL_LIGHT0, GL_ESPe, diff);
	//printf("cheguei aqui2\n");

	
	//Material
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE,matt );
	//glMaterialfv(GL_FRONT, GL_SPECULAR, matt);

	Objecto aux;
	glPushMatrix();
	aux = objectos[0];
	coords = aux.getCoords();
	normais = aux.getNormais();
	textura = aux.getTexture();
	buffer[0] = aux.getVBOBuffer();
	buffer[1] = aux.getNormalVBOBuffer();
	buffer[2] = aux.getTextureVBOBuffer();
	texID = aux.getTextid();
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

		coords = aux.getCoords();
		normais = aux.getNormais();
		textura = aux.getTexture();
		//drawScreen();
		buffer[0] = aux.getVBOBuffer();
		buffer[1] = aux.getNormalVBOBuffer();
		buffer[2] = aux.getTextureVBOBuffer();
		texID = aux.getTextid();
		//loadTexture("");
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
				texture_filename = filename->Attribute("texture");
				std::string str = filename->Attribute("file");
				guardaCoordenadas(str.c_str());
				filename = filename->NextSiblingElement();
			}
			guardaObjecto();
		}
	}
}

void initGL() {

	// alguns settings para OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);

	// init
	//converte();
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_TEXTURE_2D);
	//preparaCilindro(2, 1, lados);
}

int main(int argc, char **argv) {
	int menu1;
	// inicialização
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1000, 500);
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
	glPolygonMode(GL_FRONT, GL_FILL);
	/*Activa Luz//Luzes
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);*/
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	//Iniciar o glew 
	glewInit();

	//iniciar o devil e outras opções adicionais
	initGL();
	ilInit();
	//loadTexture();


	
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
