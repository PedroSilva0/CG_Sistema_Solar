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
#include "luz.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstring>
#pragma comment(lib,"glew32.lib")
#pragma comment(lib, "devil.lib")

//CAMARA
float alpha = 0.0, beta = 0.0; // angulos para a orientacao
float px = 0, py = 10, pz = 100; // ponto inicial da camara
float dx = 0.0, dy = 0.0, dz = 1.0; // direccao da camara

//variaveis temporárias
float px_c = 0, py_c = 0, pz_c = 0;    //Coordenadas para um ponto de cat-mull de um objecto
float rt_tempo = 0;						//tempo de rotação de um objecto
float px2 = 0, py2 = 0, pz2 = 0;		//coordenadas de translação global controladas pelo utilizador
float eixoX = 0, eixoY = 0, eixoZ = 0;  //coordenadas de rotação de um objecto
float sX = 1, sY = 1, sZ = 1;			//coordenadas de escala de um objecto
float angulo2 = 0, angulo3 = 0;			//angulo de rotaçao global controlado pelo utilizador
std::vector<Objecto> objectos;			//objectos a desenhar
std::vector<float> coords;				//coordenadas de um objecto
std::vector<float> normais;				//valores das normais de um objecto
std::vector<float> textura;				//valores da textura de um objecto
std::vector<char> transfor;				//ordem das transformações de um objecto
std::string texture_filename;			//nome do ficheiro imagem da textura

//LUZ
std::vector<Luz> luzes;					//luzes da imagem
float pos[4] = { 0,0,0,0 },				//posição das luzes por defeito
amb[3] = { 0,0,0 },						//cor da luz ambiente por defeito
diff[3] = { 0,0,0 },					//cor da difusão por defeito
matt[4] = { 0,0,0,0 };					//valores dos materiais por defeito

//catmull
float tr_tempo = 0;                       //tempo da translação de um objecto
std::vector<float> catmull_pontos;		//pontos a usar no catmull

//Para ver o numero de FPS
int times, timebase, frame = 0, fps = 0;
char print[20] = "";

//VBO
GLuint buffer[3];  // 0 é coordenadas, 1 é normais,2 e textura

//devil
GLuint texID;				//textura de um objecto


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
	glNormalPointer(GL_FLOAT, 0, 0);

	//VBO de texturas
	glBindBuffer(GL_ARRAY_BUFFER, buffer[2]);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);
	glBindTexture(GL_TEXTURE_2D, texID);

	glDrawArrays(GL_TRIANGLES, 0, coords.size() / 3);

	glBindTexture(GL_TEXTURE_2D, 0);
}

/*Guarda as coordenadas,normais e coordenadas de textura em vetores dinâmicos*/
void guardaCoordenadas(const char* filename) {
	int linhas = 0;
	std::string line;
	std::ifstream infile(filename);
	if (infile.is_open()) {
		while (getline(infile, line)) {
			if (linhas % 3 == 0) {    //linha divisivél por 3 é um ponto
				linhas++;
				char * cstr = new char[line.length() + 1];
				strcpy(cstr, line.c_str());
				char * p = strtok(cstr, " ");
				while (p != 0) {
					coords.push_back(atof(p));
					p = strtok(NULL, " ");
				}
			}
			else if (linhas % 3 == 1) {  //linhas com resto 1 quando divididas por 3 são uma normal
				linhas++;
				char * cstr = new char[line.length() + 1];
				strcpy(cstr, line.c_str());
				char * p = strtok(cstr, " ");
				while (p != 0) {
					normais.push_back(atof(p));
					p = strtok(NULL, " ");
				}
			}
			else {				//restantes são coordenadas de textura
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
}

/*Guarda objecto*/
void guardaObjecto() {
	Objecto obj;
	obj.loadTexture(texture_filename);  //carrega textura
	obj.setMaterial(matt[0], matt[1], matt[2], matt[3]);			//valores de material
	matt[0] = matt[1] = matt[2] = matt[3] = 0;		//reset de valores de material
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


void renderScene(void) {
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	// set the camera
	gluLookAt(px, py, pz,
		px - dx, py - dy, pz - dz,
		0.0f, 1.0f, 0.0f);

	//transformações globais controladas pelo utilizador
	glTranslatef(px2, py2, pz2);
	glRotatef(angulo3, 0, 1, 0);
	glRotatef(angulo2, 1, 0, 0);
	glRotatef(180, 0, 0, 1);

	//por as luzes
	Luz l;

	for (size_t k = 0; k < luzes.size(); k++) {
		l = luzes[k];
		l.getPos(pos);
		l.getAmb(amb);
		l.getDif(diff);
		glLightfv(GL_LIGHT0 + k, GL_POSITION, pos);
		glLightfv(GL_LIGHT0 + k, GL_AMBIENT, amb);
		glLightfv(GL_LIGHT0 + k, GL_DIFFUSE, diff);
	}

	Objecto aux;
	// Desenhar objectos

	for (size_t i = 0; i < objectos.size(); i++) {
		aux = objectos[i];

		glPushMatrix();
		//Posição da terra
		if (i == 4) {
			Objecto terra = objectos[3];
			float cat_trans[3];
			terra.get_cat_trans(glutGet(GLUT_ELAPSED_TIME), cat_trans);
			glTranslatef(cat_trans[0], cat_trans[1], cat_trans[2]);

		}

		//Fazer trasformações do objecto
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
		//Buscar os valores do objecto a desenhar
		coords = aux.getCoords();
		normais = aux.getNormais();
		textura = aux.getTexture();
		buffer[0] = aux.getVBOBuffer();
		buffer[1] = aux.getNormalVBOBuffer();
		buffer[2] = aux.getTextureVBOBuffer();
		texID = aux.getTextid();
		aux.getMaterial(matt);
		glMaterialfv(GL_FRONT, GL_SPECULAR, matt);
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

// funções de processamento do teclado

void teclado(unsigned char tecla, int x, int y) {
	switch (tecla) {
	case 'd':px2 = px2 + 1; break;  //mexer para direita
	case 'a':px2 = px2 - 1; break;	//mexer para esquerda
	case 'w':py2 = py2 + 1; break;  //mexer para cima
	case 's':py2 = py2 - 1; break;  //mexer para baixo
	case 'j': {angulo3 = angulo3 - 1; } break; //roda para a esquerda
	case 'l': {angulo3 = angulo3 + 1; } break; //roda para a direira
	case 'i': {angulo2 = angulo2 + 1; } break; //roda para baixo
	case 'k': {angulo2 = angulo2 - 1; } break; //roda para cima
	default:
		break;
	}
	glutPostRedisplay();
}

void teclaCamara(int tecla, int x, int y) {
	float k = 0;
	switch (tecla) {
		// rotacao da camara na hor (+)
	case GLUT_KEY_RIGHT: alpha -= 0.02; break;
		// rotacao da camara na hor (−)
	case GLUT_KEY_LEFT: alpha += 0.02; break;
		// rotacao da camara na vert (−)
	case GLUT_KEY_DOWN: beta += 0.02;
		if (beta > 1.5) beta = 1.5;
		if (beta < -1.5) beta = -1.5;
		break;
		// rotacao da camara na vert (+)
	case GLUT_KEY_UP: beta -= 0.02;
		if (beta > 1.5) beta = 1.5;
		if (beta < -1.5) beta = -1.5;
		break;
		// aproximacao da camara
	case GLUT_KEY_PAGE_UP: k -= 1; break;
		// afastamento da camara
	case GLUT_KEY_PAGE_DOWN: k += 1; break;

	}

	dx = cos(beta) * sin(alpha);
	dy = sin(beta);
	dz = cos(beta) * cos(alpha);

	px = px + k*dx;
	py = py + k*dy;
	pz = pz + k*dz;

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


//Adicionar luzes
void addLuz() {
	Luz l;
	//Posição
	l.setPos(pos[0], pos[1], pos[2], pos[3]);
	pos[0] = pos[1] = pos[2] = pos[3] = 0;
	//Ambiente
	l.setAmb(amb[0], amb[1], amb[2]);
	amb[1] = amb[2] = amb[0] = 0;
	//Difusão
	l.setDiff(diff[0], diff[1], diff[2]);
	diff[0] = diff[1] = diff[2] = 0;
	//guardar luz
	luzes.push_back(l);
}

//Tratar do xml
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
				name->QueryFloatAttribute("diffR", &matt[0]);
				name->QueryFloatAttribute("diffG", &matt[1]);
				name->QueryFloatAttribute("diffB", &matt[2]);
				guardaCoordenadas(str.c_str());
				filename = filename->NextSiblingElement();
			}
			guardaObjecto();
		}
		if (!strcmp(name->Value(), "lights")) {
			handleGrupo(pNode->FirstChild());
			handleGrupo(pNode->NextSibling());
		}
		if (!strcmp(name->Value(), "light")) {
			std::string str = name->Attribute("type");

			name->QueryFloatAttribute("posX", &pos[0]);
			name->QueryFloatAttribute("posY", &pos[1]);
			name->QueryFloatAttribute("posZ", &pos[2]);
			if (!strcmp(str.c_str(), "POINT")) {
				pos[3] = 1;
			}
			else {
				pos[3] = 0;
			}
			handleGrupo(pNode->FirstChild());
			addLuz();
			handleGrupo(pNode->NextSibling());
		}

		if (!strcmp(name->Value(), "diffuse")) {
			name->QueryFloatAttribute("diffR", &diff[0]);
			name->QueryFloatAttribute("diffG", &diff[1]);
			name->QueryFloatAttribute("diffB", &diff[2]);
			handleGrupo(pNode->NextSibling());
		}
		if (!strcmp(name->Value(), "ambient")) {
			name->QueryFloatAttribute("ambR", &amb[0]);
			name->QueryFloatAttribute("ambG", &amb[1]);
			name->QueryFloatAttribute("ambB", &amb[2]);
			handleGrupo(pNode->NextSibling());
		}

	}
}

//Ativar luzes individuais
void enableLuz() {
	for (size_t i = 0; i < luzes.size(); i++) {
		glEnable(GL_LIGHT0 + i);
	}
}

//alguns valores do glut
void initGL() {

	// alguns settings para OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_LIGHTING);

	glEnable(GL_TEXTURE_2D);
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
	glutSpecialFunc(teclaCamara);
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

	//Iniciar o glew 
	glewInit();

	//iniciar o devil e outras opções adicionais
	initGL();
	ilInit();

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
		doc.LoadFile();
		if (doc.Error()) {
			printf("Error in %s: %s\n", doc.Value(), doc.ErrorDesc());
			exit(1);
		}
		TiXmlNode* node = 0;
		node = doc.RootElement();
		handleGrupo(node->FirstChild());
	}
	//ativar luzes
	enableLuz();

	// entrar no ciclo do GLUT 
	glutMainLoop();

	return 1;
}
