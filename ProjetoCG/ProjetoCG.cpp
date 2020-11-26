// Trabalho realizado por: Jose Esperanca, 2018278596
//===================================================
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/Glut.h>

//--------------------------------- Definir cores
#define BLUE     0.0, 0.0, 1.0, 1.0
#define RED		 1.0, 0.0, 0.0, 1.0
#define YELLOW	 1.0, 1.0, 0.0, 1.0
#define GREEN    0.0, 1.0, 0.0, 1.0
#define ORANGE   1.0, 0.5, 0.1, 1.0
#define CYAN     0.0, 1.0, 1.0, 1.0
#define WHITE    1.0, 1.0, 1.0, 1.0
#define BLACK    0.0, 0.0, 0.0, 1.0
#define GRAY     0.3, 0.3, 0.3, 1.0
#define PI		 3.14159

#define MAXTRANSLACAO 2.20
#define MAXROTACAO 90
#define MAXROTMACANETA 45

//================================================================================
//===========================================================Variaveis e constantes
GLint     msec = 10;
char      texto[30];						//.. texto a ser escrito no ecran

//------------------------------------------------------------ Sistema Coordenadas + objectos
GLint		wScreen = 600, hScreen = 500;		//.. janela
GLfloat		yC = 100.0;	//.. Mundo

//------------------------------------------------------------ Observador 
GLfloat  rVisao = 12, aVisao = PI, incVisao = 0.05;
GLfloat  obsP[] = { rVisao * cos(aVisao), 3.0, rVisao * sin(aVisao) };
GLfloat  obsT[] = { obsP[0] - rVisao * cos(aVisao), obsP[1], obsP[2] - rVisao * sin(aVisao) };

float anguloZ = 95;
int   tipoProjeccao = 0;
GLfloat anguloPorta = 0.0;
GLfloat anguloMacaneta = 0.0;
GLfloat posicao = 0.0;
GLfloat tam = 0.5;


//--------------------------------------------- Escada
static GLuint    faceSuperior[] = { 0, 3, 2, 1 };
static GLuint    faceInferior[] = { 4, 5, 6, 7 };
static GLuint    faceEsquerda[] = { 0, 4, 7, 3 };
static GLuint    faceDireita[] = { 1, 2, 6, 5 };
static GLuint    faceFrontal[] = { 0, 1, 5, 4 };
static GLuint    FaceTraseira[] = { 2, 3, 7, 6 };

static GLfloat vertices[] = {
	-tam, tam, tam,	  // 0
	-tam, tam, -tam,  // 1
	tam, tam, -tam,	  // 2
	tam, tam, tam,	  // 3
	-tam, -tam, tam,  // 4
	-tam, -tam, -tam, // 5
	tam, -tam, -tam,  // 6
	tam, -tam, tam	  // 7
};

static GLfloat normais[] = {

	0.0, 1.0, 0.0,	  // 0
	0.0, 1.0, 0.0,	  // 1
	0.0, 1.0, 0.0,	  // 2
	0.0, 1.0, 0.0,	  // 3
	0.0, 1.0, 0.0,	  // 4
	0.0, 1.0, 0.0,	  // 5
	0.0, 1.0, 0.0,	  // 6
	0.0, 1.0, 0.0,	  // 7
};


//================================================================================
//=========================================================================== INIT
void initialize(void)
{
	glClearColor(BLACK);		//………………………………………………………………………………Apagar
	glEnable(GL_DEPTH_TEST);	//………………………………………………………………………………Profundidade
	glShadeModel(GL_SMOOTH);	//………………………………………………………………………………Interpolacao de cores	

	glEnable(GL_CULL_FACE);		//………………………………………………………………………………Faces visiveis
	glCullFace(GL_BACK);		//………………………………………………………………………………Mostrar so as da frente

	glVertexPointer(3, GL_FLOAT, 0, vertices); //………………………………………Vertex arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 0, normais);
	glEnableClientState(GL_NORMAL_ARRAY);

	srand(1);
}


float aleatorio(GLint minimo, GLint maximo) {
	GLfloat y;
	y = rand() % 1000;
	return (minimo + 0.001 * y * (maximo - minimo));
}


void desenhaTexto(char* string, GLfloat x, GLfloat y)
{
	glPushMatrix();
	glTranslatef(x, 0, y);
	glRasterPos2f(0, 0);
	while (*string)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *string++);
	glPopMatrix();
}


void drawEixos()
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixo
	glColor4f(ORANGE);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(10, 0, 0);
	glEnd();
	glColor4f(GREEN);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 10, 0);
	glEnd();
	glColor4f(CYAN);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 0, 10);
	glEnd();

}


void drawObservador_Local()
{
	glColor4f(GREEN);
	glPushMatrix();
	glTranslatef(obsP[0], obsP[1], obsP[2]);
	glPopMatrix();
}


void drawObservador_Target()
{
	glColor4f(YELLOW);
	glPushMatrix();
	glTranslatef(obsT[0], obsT[1], obsT[2]);
	glPopMatrix();
}


void draw_Informacao()
{
	if (tipoProjeccao)
		sprintf_s(texto, 30, "Projecao = Ortogonal");
	else
		sprintf_s(texto, 30, "Projecao = Perspectiva");
	desenhaTexto(texto, -15, -15);
}


void desenhaCubo(GLfloat x, GLfloat y, GLfloat z, GLfloat scalex, GLfloat scaley, GLfloat scalez, GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
	
	glPushMatrix();
	
	glColor4f(r, g, b, a);
	glTranslatef(x, y, z);
	glScalef(scalex, scaley, scalez);

	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, faceSuperior);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, faceInferior);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, faceEsquerda);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, faceDireita);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, faceFrontal);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, FaceTraseira);

	glPopMatrix();
}


void desenhaPorta() {
	
	// Porta
	desenhaCubo(2.0, 2.5, 0.3, 4.0, 5.0, 0.5, 0.9725, 0.8745, 0.6314, 1.0); // Parte Baixo
	desenhaCubo(0.25, 7.25, 0.3, 0.5, 4.5, 0.5, 0.9725, 0.8745, 0.6314, 1.0); // Parte Esquerda
	desenhaCubo(3.75, 7.25, 0.3, 0.5, 4.5, 0.5, 0.9725, 0.8745, 0.6314, 1.0); // Parte Direita
	desenhaCubo(2.0, 9.75, 0.3, 4.0, 0.5, 0.5, 0.9725, 0.8745, 0.6314, 1.0); // Parte Cima

	// Janela Fixa
	desenhaCubo(2.0, 9.45, 0.4, 3.0, 0.1, 0.1, 0.5, 0.5, 0.5, 1.0); // Parte Cima
	desenhaCubo(2.0, 8.35, 0.4, 0.1, 2.1, 0.1, 0.5, 0.5, 0.5, 1.0); // Parte Vertical
	desenhaCubo(2.0, 7.25, 0.4, 3.0, 0.1, 0.1, 0.5, 0.5, 0.5, 1.0); // Parte Meio
}


void desenhaJanela() {

	desenhaCubo(2.0, 7.25 + posicao, 0.3, 3.0, 0.1, 0.1, 0.5, 0.5, 0.5, 1.0); // Parte Cima
	desenhaCubo(2.0, 6.15 + posicao, 0.3, 0.1, 2.1, 0.1, 0.5, 0.5, 0.5, 1.0); // Parte Vertical
	desenhaCubo(2.0, 5.05 + posicao, 0.3, 3.0, 0.1, 0.1, 0.5, 0.5, 0.5, 1.0); // Parte Baixo
}


void desenhaMacaneta() {

	glPushMatrix();

	glColor4f(0.5, 0.5, 0.5, 1.0);
	glTranslatef(3.3, 4.15, 0.6);
	glRotatef(anguloMacaneta, 0.0, 0.0, 1.0);
	glScalef(1.0, 0.3, 0.2);

	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, faceSuperior);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, faceInferior);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, faceEsquerda);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, faceDireita);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, faceFrontal);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, FaceTraseira);

	glPopMatrix();
}


//======================================
void drawScene() {

	glPushMatrix();
	glRotatef(-anguloPorta, 0.0, 1.0, 0.0);
	desenhaPorta();
	desenhaJanela();
	desenhaMacaneta();
	glPopMatrix();
}


//====================================================== 
//====================================================== 
void display(void) {

	//================================================================= APaga 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//================================================================= Viewport 1 MAPA
	// projecao ortogonal
	glViewport(0, 0, 0.25 * wScreen, 0.25 * hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-20, 20, -20, 20, -100, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 30, 0, 0, 0, 0, 0, 0, -1);
	//…………………………………………………………………………………………………………………………………………………………Objectos
	drawEixos();
	drawObservador_Local();
	drawObservador_Target();
	drawScene();
	draw_Informacao();

	//================================================================= Viewport 2
	glViewport(0.25 * wScreen, 0.25 * hScreen, 0.75 * wScreen, 0.75 * hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (tipoProjeccao == 0)
		gluPerspective(anguloZ, (float)wScreen / hScreen, 0.1, 9999);
	if (tipoProjeccao == 1)
		glOrtho(-20, 20, -20, 20, -100, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(obsP[0], obsP[1], obsP[2], obsT[0], obsT[1], obsT[2], 0, 1, 0);

	//…………………………………………………………………………………………………………………………………………………………Objectos
	drawEixos();
	drawScene();

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Actualizacao
	glutSwapBuffers();
}


void Timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(msec, Timer, 1);
}

//======================================================= EVENTOS
void keyboard(unsigned char key, int x, int y) {


	switch (key) {

		//------------------------------ zoom
	case 'z': 	case 'Z':
		rVisao -= 1;
		break;

	case 'x':	case 'X':
		rVisao += 1;
		break;

		//------------------------------ projeccao
	case 'p': 	case 'P':
		tipoProjeccao = (tipoProjeccao + 1) % 2;
		glutPostRedisplay();
		break;

		//--------------------------- Escape
	case 27:
		exit(0);
		break;

		//-------------------------- Abrir Janela
	case 's':	case 'S':
		if (posicao < MAXTRANSLACAO)
			posicao += 0.1;
		break;

	case 'd':	case 'D':
		if (posicao > 0.0)
			posicao -= 0.1;
		break;

		//-------------------------- Abrir Porta
	case 'w': case 'W':
		if (anguloPorta == 0.0 && anguloMacaneta == MAXROTMACANETA)
			anguloPorta += 3.0;
		else if (anguloPorta > 0.0 && anguloPorta < MAXROTACAO)
			anguloPorta += 3.0;
		break;

	case 'e': case 'E':
		if (anguloPorta > 0.0)
			anguloPorta -= 3.0;
		break;

		//-------------------------- Rodar Maçaneta
	case 'q': case 'Q':
		if (anguloMacaneta < MAXROTMACANETA)
			anguloMacaneta += 5.0;
		break;

	case 'a': case 'A':
		if (anguloMacaneta > 0.0)
			anguloMacaneta -= 5.0;
		break;
	}
}



void teclasNotAscii(int key, int x, int y) {


	//------------------------------- olhar para a origem
	obsT[0] = 0; obsT[1] = 0; obsT[2] = 0;

	if (key == GLUT_KEY_UP)    obsP[1] = obsP[1] + 0.5;
	if (key == GLUT_KEY_DOWN)  obsP[1] = obsP[1] - 0.5;
	if (key == GLUT_KEY_LEFT)  aVisao = aVisao + 0.1;
	if (key == GLUT_KEY_RIGHT) aVisao = aVisao - 0.1;

	if (obsP[1] > yC)   obsP[1] = yC;
	if (obsP[1] < -yC)  obsP[1] = -yC;
	obsP[0] = rVisao * cos(aVisao);
	obsP[2] = rVisao * sin(aVisao);

	glutPostRedisplay();

}


//======================================================= MAIN
//======================================================= MAIN
int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(wScreen, hScreen);
	glutInitWindowPosition(400, 100);
	glutCreateWindow("Observador:'SETAS'|  |Projecao -'p'|  |Janela -'s', 'd'|  |Abrir -'w', 'e'|  |Maçaneta -'q', 'a'");

	initialize();

	glutSpecialFunc(teclasNotAscii);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(msec, Timer, 1);

	glutMainLoop();

	return 0;
}