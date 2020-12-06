// Trabalho realizado por: Jose Esperanca, 2018278596
//===================================================
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/Glut.h>
#include "RgbImage.h"

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
//============================================================ Variaveis e constantes
GLint	msec = 10;
char	texto[30];
char	keys[100];

//------------------------------------------------------------ Sistema Coordenadas + objectos
GLint	wScreen = 600, hScreen = 500;						
GLfloat	yC = 100.0;											

//------------------------------------------------------------ Observador 
GLfloat rVisao = 8, aVisao = PI, incVisao = 0.05;
GLfloat obsP[] = { rVisao * cos(aVisao), 4.0, rVisao * sin(aVisao) };
GLfloat obsT[] = { obsP[0] - rVisao * cos(aVisao), obsP[1], obsP[2] - rVisao * sin(aVisao) };

float	anguloZ = 95;
int		tipoProjeccao = 0;
GLfloat anguloPorta = 0.0;
GLfloat anguloMacaneta = 0.0;
GLfloat posicao = 0.0;
GLfloat tam = 0.5;

//------------------------------------------------------------ Texturas 
GLuint   texture[3];
RgbImage imag;
int pol_incr = 1;

//------------------------------------------------------------ Luzes 
GLfloat intensidadeDia = 0.3;

int		enable_l0 = 1;
GLfloat pontPos[4] = { 2.0, 9.0, 3.0, 1.0 };
GLfloat pontCorAmb[4] = { 0, 0, 0, 0.0 };
GLfloat pontCorDif[4] = { 1, 1, 1, 1.0 };
GLfloat pontCorEsp[4] = { 1, 1, 1, 1.0 };

int		enable_l1 = 1;
GLfloat focoR = 1;
GLfloat focoG = 1;
GLfloat focoB = 1;
GLfloat focoDirecao[4] = { 0, -1, 0, 0 };

GLfloat	fogColor[] = { 0.3, 0.3, 0.3, 1.0 };
//------------------------------------------------------------ Materials

GLfloat	silverAmb[] = { 0.19225, 0.19225, 0.19225 };
GLfloat	silverDif[] = { 0.50754, 0.50754, 0.50754 };
GLfloat	silverSpe[] = { 0.508273, 0.508273, 0.508273 };
GLint	silverShi = 0.5 * 128;

GLfloat	woodAmb[] = { 0.25, 0.148, 0.06475 };
GLfloat	woodDif[] = { 0.4, 0.2368, 0.1036 };
GLfloat	woodSpe[] = { 0.774597, 0.458561, 0.200621 };
GLint	woodShi = 0.05 * 128;

GLfloat glassAmb[] = { 0.1, 0.1, 0.1, 0.5 };
GLfloat glassDif[] = { 0.1, 0.1, 0.1, 0.5 };
GLfloat	glassSpe[] = { 0.1, 0.1, 0.1, 0.5 };
GLint	glassShi = 0.1 * 128;

GLfloat poorcelanaAmb[] = { 1, 1, 1 };
GLfloat poorcelanaDif[] = { 1.0, 1, 1 };
GLfloat	poorcelanaSpeCoef = 1;
GLint	poorcelanaShi = 0.3 * 128;

//================================================================================
static GLuint    faceSuperior[] = { 0, 1, 2, 3 };
static GLuint    faceInferior[] = { 4, 5, 6, 7 };
static GLuint    faceFrontal[] = { 8, 9, 10, 11 };
static GLuint    faceTraseira[] = { 12, 13, 14, 15 };
static GLuint    faceEsquerda[] = { 16, 17, 18, 19 };
static GLuint    faceDireita[] = { 20, 21, 22, 23 };

static GLfloat vertices[] = {

	// Face Superior
	-tam, tam, tam,	  // 0
	tam, tam, tam,	  // 1
	tam, tam, -tam,	  // 2
	-tam, tam, -tam,  // 3

	// Face Inferior
	-tam, -tam, tam,  // 4
	-tam, -tam, -tam, // 5
	tam, -tam, -tam,  // 6
	tam, -tam, tam,	  // 7

	// Face Frontal
	-tam, tam, tam,   // 8
	-tam, -tam, tam,  // 9
	tam, -tam, tam,	  // 10
	tam, tam, tam,	  // 11

	// Face Traseira
	-tam, tam, -tam,  // 12
	tam, tam, -tam,	  // 13
	tam, -tam, -tam,  // 14
	-tam, -tam, -tam, // 15

	// Face Esquerda
	-tam, tam, tam,	  // 16
	-tam, tam, -tam,  // 17
	-tam, -tam, -tam, // 18
	-tam, -tam, tam,  // 19

	// Face Direita
	tam, tam, -tam,	  // 20
	tam, tam, tam,	  // 21
	tam, -tam, tam,	  // 22
	tam, -tam, -tam,  // 23
};

static GLfloat normais[] = {

	// Face Superior
	0.0, 1.0, 0.0,	  // 0
	0.0, 1.0, 0.0,	  // 1
	0.0, 1.0, 0.0,	  // 2
	0.0, 1.0, 0.0,	  // 3

	// Face Inferior
	0.0, -1.0, 0.0,	  // 4
	0.0, -1.0, 0.0,	  // 5
	0.0, -1.0, 0.0,	  // 6
	0.0, -1.0, 0.0,	  // 7
	
	// Face Frontal
	0.0, 0.0, 1.0,	  // 8
	0.0, 0.0, 1.0,	  // 9
	0.0, 0.0, 1.0,	  // 10
	0.0, 0.0, 1.0,	  // 11
	
	// Face Traseira
	0.0, 0.0, -1.0,	  // 12
	0.0, 0.0, -1.0,	  // 13
	0.0, 0.0, -1.0,	  // 14
	0.0, 0.0, -1.0,	  // 15
	
	// Face Esquerda
	-1.0, 0.0, 0.0,	  // 16
	-1.0, 0.0, 0.0,	  // 17
	-1.0, 0.0, 0.0,	  // 18
	-1.0, 0.0, 0.0,	  // 19
	
	// Face Direita
	1.0, 0.0, 0.0,	  // 20
	1.0, 0.0, 0.0,	  // 21
	1.0, 0.0, 0.0,	  // 22
	1.0, 0.0, 0.0,	  // 23

};

static GLfloat texturas[] = {

	// Face Superior
	0, 0,	// 0
	1, 0,	// 1
	1, 1,	// 2
	0, 1,	// 3

	// Face Inferior
	0, 0,	// 4
	0, 1,	// 5
	1, 1,	// 6
	1, 0,	// 7

	// Face Frontal
	0, 1,	// 8
	0, 0,	// 9
	1, 0,	// 10
	1, 1,	// 11

	// Face Traseira
	1, 1,	// 12
	0, 1,	// 13
	0, 0,	// 14
	1, 0,	// 15

	// Face Esquerda
	1, 1,	// 16
	0, 1,	// 17
	0, 0,	// 18
	1, 0,	// 19

	// Face Direita
	1, 1,	// 20
	0, 1,	// 21
	0, 0,	// 22
	1, 0,	// 23
};


//================================================================================
//=========================================================================== INIT

void initTexturas()
{
	//LOAD MADEIRA
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	imag.LoadBmpFile("madeira1.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

	//LOAD METAL
	glGenTextures(1, &texture[1]);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	imag.LoadBmpFile("metal.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

	//LOAD TILE
	glGenTextures(1, &texture[2]);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	imag.LoadBmpFile("tile.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
}


void initLights(void) {

	GLfloat luzGlobalCorAmb[4] = { intensidadeDia, intensidadeDia,intensidadeDia, 1.0 };
	GLfloat focoPos[4] = { obsT[0], obsT[1], obsT[2], 1.0 };
	GLfloat focoCorDif[4] = { focoR, focoG, focoB, 1.0 };
	GLfloat focoCorEsp[4] = { focoR, focoG, focoB, 1.0 };

	//…………………………………………………………………………………………………………………………………………… Ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCorAmb);

	//…………………………………………………………………………………………………………………………………………… Teto
	glLightfv(GL_LIGHT0, GL_POSITION, pontPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, pontCorAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, pontCorDif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, pontCorEsp);

	//…………………………………………………………………………………………………………………………………………… Foco
	glLightfv(GL_LIGHT1, GL_POSITION, focoPos);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, focoCorDif);
	glLightfv(GL_LIGHT1, GL_SPECULAR, focoCorEsp);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, focoDirecao);

	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0f); 
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.05f);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0f);
}


void iluminacao() {

	GLfloat luzGlobalCorAmb[4] = { intensidadeDia, intensidadeDia,intensidadeDia, 1.0 };
	GLfloat focoPos[4] = { obsT[0], obsT[1], obsT[2], 1.0 };
	GLfloat focoCorDif[4] = { focoR, focoG, focoB, 1.0 };
	GLfloat focoCorEsp[4] = { focoR, focoG, focoB, 1.0 };

	//…………………………………………………………………………………………………………………………………………… Ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCorAmb);

	//…………………………………………………………………………………………………………………………………………… Teto
	if (enable_l0 == 1){
		glLightfv(GL_LIGHT0, GL_POSITION, pontPos);
		glLightfv(GL_LIGHT0, GL_AMBIENT, pontCorAmb);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, pontCorDif);
		glLightfv(GL_LIGHT0, GL_SPECULAR, pontCorEsp);
	}

	//…………………………………………………………………………………………………………………………………………… Foco
	if (enable_l1 == 1) {
		glLightfv(GL_LIGHT1, GL_POSITION, focoPos);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, focoCorDif);
		glLightfv(GL_LIGHT1, GL_SPECULAR, focoCorEsp);
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, focoDirecao);

		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30);
		glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0f);
		glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f);
		glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.05f);
		glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0f);
	}
}


void initialize(void)
{
	glClearColor(BLACK);		//………………………………………………………………………………Apagar
	glShadeModel(GL_SMOOTH);	//………………………………………………………………………………Interpolacao de cores	
	glEnable(GL_DEPTH_TEST);	//………………………………………………………………………………Profundidade
	glEnable(GL_BLEND);

	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_EXP2);
	glFogf(GL_FOG_DENSITY, 0.05);
	glFogfv(GL_FOG_COLOR, fogColor);

	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	initLights();

	initTexturas();

	glEnable(GL_CULL_FACE);		//………………………………………………………………………………Faces visiveis
	glCullFace(GL_BACK);		//………………………………………………………………………………Mostrar so as da frente

	glVertexPointer(3, GL_FLOAT, 0, vertices); //………………………………………Vertex arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 0, normais);
	glEnableClientState(GL_NORMAL_ARRAY);

	glTexCoordPointer(2, GL_FLOAT, 0, texturas);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	srand(1);
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


void draw_Informacao()
{
	glDisable(GL_LIGHTING);
	if (tipoProjeccao)
		sprintf_s(texto, 30, "Projecao = Ortogonal");
	else
		sprintf_s(texto, 30, "Projecao = Perspectiva");

	desenhaTexto(texto, -17, -20);
	glEnable(GL_LIGHTING);
}


void desenhaCubo(GLfloat x, GLfloat y, GLfloat z, GLfloat scalex, GLfloat scaley, GLfloat scalez, GLfloat amb[], GLfloat dif[], GLfloat spe[], GLfloat shi, int indice) {
	
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spe);
	glMaterialf(GL_FRONT, GL_SHININESS, shi);

	glTranslatef(x, y, z);
	glScalef(scalex, scaley, scalez);

	glBindTexture(GL_TEXTURE_2D, texture[indice]);

	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, faceSuperior);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, faceInferior);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, faceFrontal);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, faceTraseira);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, faceEsquerda);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, faceDireita);

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}


void drawObservador_Local()
{
	desenhaCubo(obsP[0], obsP[1], obsP[2], 1.0, 1.0, 1.0, woodAmb, woodDif, woodSpe, woodShi, 2);
}


void drawObservador_Target()
{
	//glDisable(GL_LIGHTING);
	desenhaCubo(obsT[0], obsT[1], obsT[2], 0.5, 0.5, 0.5, silverAmb, silverDif, silverSpe, silverShi, 2);
	glEnable(GL_LIGHTING);
}


void desenhaPorta() {

	// Porta
	desenhaCubo(2.0, 2.5, 0.3, 4.0, 5.0, 0.5, woodAmb, woodDif, woodSpe, woodShi, 0); // Parte Baixo
	desenhaCubo(0.25, 7.25, 0.3, 0.5, 4.5, 0.5, woodAmb, woodDif, woodSpe, woodShi, 0); // Parte Esquerda
	desenhaCubo(3.75, 7.25, 0.3, 0.5, 4.5, 0.5, woodAmb, woodDif, woodSpe, woodShi, 0); // Parte Direita
	desenhaCubo(2.0, 9.75, 0.3, 4.0, 0.5, 0.5, woodAmb, woodDif, woodSpe, woodShi, 0); // Parte Cima
}


void desenhaJanela() {

	// Janela Fixa
	desenhaCubo(2.0, 9.45, 0.4, 3.0, 0.1, 0.1, silverAmb, silverDif, silverSpe, silverShi, 1); // Parte Cima
	desenhaCubo(2.0, 8.35, 0.4, 0.1, 2.1, 0.1, silverAmb, silverDif, silverSpe, silverShi, 1); // Parte Vertical
	desenhaCubo(2.0, 7.25, 0.4, 3.0, 0.1, 0.1, silverAmb, silverDif, silverSpe, silverShi, 1); // Parte Meio

	desenhaCubo(2.0, 7.25 + posicao, 0.3, 3.0, 0.1, 0.1, silverAmb, silverDif, silverSpe, silverShi, 1); // Parte Cima
	desenhaCubo(2.0, 6.15 + posicao, 0.3, 0.1, 2.1, 0.1, silverAmb, silverDif, silverSpe, silverShi, 1); // Parte Vertical
	desenhaCubo(2.0, 5.05 + posicao, 0.3, 3.0, 0.1, 0.1, silverAmb, silverDif, silverSpe, silverShi, 1); // Parte Baixo
}


void desenhaMacaneta() {

	glEnable(GL_TEXTURE_2D);
	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, silverAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, silverDif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, silverSpe);
	glMaterialf(GL_FRONT, GL_SHININESS, silverShi);

	glTranslatef(3.3, 4.15, 0.6);
	glRotatef(anguloMacaneta, 0.0, 0.0, 1.0);
	glScalef(1.0, 0.3, 0.2);

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, faceSuperior);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, faceInferior);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, faceFrontal);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, faceTraseira);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, faceEsquerda);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, faceDireita);

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}


void desenhaMalha() {

	int		i, j;
	int		dim = 32;
	float	med_dim = (float)dim*pol_incr / 2;
	GLfloat poorcelanaSpe[] = { 1 * poorcelanaSpeCoef, 1 * poorcelanaSpeCoef, 1 * poorcelanaSpeCoef };

	glEnable(GL_TEXTURE_2D);
	glPushMatrix();

	glTranslatef(-14, 0, -15.5);
	glScalef(med_dim, med_dim, med_dim);
	glNormal3f(0, 1, 0);           
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, poorcelanaAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, poorcelanaDif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, poorcelanaSpe);
	glMaterialf(GL_FRONT, GL_SHININESS, poorcelanaShi);

	glBegin(GL_QUADS);

	for (i = 0; i < dim; i += pol_incr)
		for (j = 0; j < dim; j += pol_incr) {
			
			glTexCoord2f(0, 0);
			glVertex3d((float)j / med_dim, 0, (float)(i + pol_incr) / med_dim);

			glTexCoord2f(0, 1);
			glVertex3d((float)(j + pol_incr) / med_dim, 0, (float)(i + pol_incr) / med_dim);

			glTexCoord2f(1, 1);
			glVertex3d((float)(j + pol_incr) / med_dim, 0, (float)i / med_dim );

			glTexCoord2f(1, 0);
			glVertex3d((float)j / med_dim, 0, (float)i / med_dim);
		}

	glDisable(GL_TEXTURE_2D);
	glEnd();
	glPopMatrix();
}


void desenhaVidros() {

	desenhaCubo(1.225, 8.35, 0.4, 1.45, 2.1, 0.1, glassAmb, glassDif, glassSpe, glassShi, 1); // Cima esquerda
	desenhaCubo(2.775, 8.35, 0.4, 1.45, 2.1, 0.1, glassAmb, glassDif, glassSpe, glassShi, 1); // Cima Direita

	desenhaCubo(1.225, 6.15 + posicao, 0.4, 1.45, 2.1, 0.1, glassAmb, glassDif, glassSpe, glassShi, 1); // Baixo Esquerda
	desenhaCubo(2.775, 6.15 + posicao, 0.4, 1.45, 2.1, 0.1, glassAmb, glassDif, glassSpe, glassShi, 1); // Baixo Direita
}


//======================================
void drawScene() {

	glPushMatrix();
	glBlendFunc(GL_ONE, GL_ZERO);
	desenhaMalha();
	glRotatef(-anguloPorta, 0.0, 1.0, 0.0);
	desenhaPorta();
	desenhaJanela();
	desenhaMacaneta();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	desenhaVidros();
	glPopMatrix();
}


//====================================================== 
//====================================================== 
void display(void) {

	//================================================================= Apaga 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//================================================================= Viewport 1 MAPA
	// projecao ortogonal
	//glDisable(GL_LIGHTING);
	glViewport(0, 0, 0.25 * wScreen, 0.25 * hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-20, 20, -20, 20, -100, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 30, 0, 0, 0, 0, 0, 0, -1);

	//…………………………………………………………………………………………………………………………………………………………Objectos
	iluminacao();
	drawEixos();
	drawObservador_Local();
	drawObservador_Target();
	drawScene();
	draw_Informacao();

	//================================================================= Viewport 2
	glEnable(GL_LIGHTING);
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
	iluminacao();
	drawObservador_Target();
	//drawEixos();
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
		//------------------------------ projeccao
	case 'p': 	case 'P':
		tipoProjeccao = (tipoProjeccao + 1) % 2;
		break;

		//--------------------------- Escape
	case 27:
		exit(0);
		break;

		//-------------------------- Abrir Janela
	case '1':
		if (posicao < MAXTRANSLACAO)
			posicao += 0.1;
		break;

	case '2':
		if (posicao > 0.0)
			posicao -= 0.1;
		break;

		//-------------------------- Abrir Porta
	case '3':
		if (anguloPorta == 0.0 && anguloMacaneta == MAXROTMACANETA)
			anguloPorta += 3.0;
		else if (anguloPorta > 0.0 && anguloPorta < MAXROTACAO)
			anguloPorta += 3.0;
		break;

	case '4':
		if (anguloPorta > 0.0)
			anguloPorta -= 3.0;
		break;

		//-------------------------- Rodar Maçaneta
	case '5':
		if (anguloMacaneta < MAXROTMACANETA)
			anguloMacaneta += 5.0;
		break;

	case '6':
		if (anguloMacaneta > 0.0)
			anguloMacaneta -= 5.0;
		break;

		//-------------------------- Toggle L0 e L1 e Amb
	case '7':
		enable_l0 = (enable_l0 + 1) % 2;
		if (enable_l0 == 1)
			glEnable(GL_LIGHT0);
		else
			glDisable(GL_LIGHT0);
		break;

	case '8':
		enable_l1 = (enable_l1 + 1) % 2;
		if (enable_l1 == 1)
			glEnable(GL_LIGHT1);
		else
			glDisable(GL_LIGHT1);
		break;

	case '9':
		intensidadeDia += 0.2;
		if (intensidadeDia > 1)
			intensidadeDia = 0;
		break;

		//-------------------------- Especular do chão

	case '0':
		poorcelanaSpeCoef += 0.1;
		if (poorcelanaSpeCoef >= 1)
			poorcelanaSpeCoef = 0.1;
		break;

		//-------------------------- Mudar cor do foco

	case 'r': case 'R':
		focoR += 0.2;
		if (focoR > 1)
			focoR = 0;
		break;

	case 'g': case 'G':
		focoG += 0.2;
		if (focoG > 1)
			focoG = 0;
		break;

	case 'b': case 'B':
		focoB += 0.2;
		if (focoB > 1)
			focoB = 0;
		break;

		//-------------------------- Malha
	case 'n': case 'N':
		if (pol_incr > 1)
			pol_incr = pol_incr / 2;
		break;

	case 'm': case 'M':
		if (pol_incr < 32)
			pol_incr = pol_incr * 2;
		break;

		//-------------------------- Movement
	case 'w': case 'W':
		obsT[1] += 1;
		break;

	case 's': case 'S':
		obsT[1] -= 1;
		break;

	case 'd': case 'D':
		obsP[1] += 0.5;
		break;

	case 'a': case 'A':
		obsP[1] -= 0.5;
		break;

	case 'e': case 'E':
		aVisao += 0.1;
		break;

	case 'q': case 'Q':
		aVisao -= 0.1;
		break;

	case 'z': 	case 'Z':
		rVisao -= 1;
		break;

	case 'x':	case 'X':
		rVisao += 1;
		break;
	}

	if (obsP[1] > yC)   obsP[1] = yC;
	if (obsP[1] < -yC)  obsP[1] = -yC;
	obsP[0] = rVisao * cos(aVisao);
	obsP[2] = rVisao * sin(aVisao);

	glutPostRedisplay();
}


void teclasNotAscii(int key, int x, int y) {

	//------------------------------- olhar para a origem

	if (key == GLUT_KEY_UP)		obsT[2] -= 1;
	if (key == GLUT_KEY_DOWN)	obsT[2] += 1;
	if (key == GLUT_KEY_LEFT)	obsT[0] -= 1;
	if (key == GLUT_KEY_RIGHT)	obsT[0] += 1;

	glutPostRedisplay();
}


//======================================================= MAIN
int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(wScreen, hScreen);
	glutInitWindowPosition(400, 100);
	glutCreateWindow("Movimentacao - 'SETAS / WASD / QE' || Janela - 1 / 2 || Abrir - 3 / 4 || Maçaneta - 5 / 6 || Luzes - 7 / 8 / 9 || Especularidade do chão - 0 || Projecao - P || Malha - M / N || Cor Foco - R / G / B");

	initialize();

	glutSpecialFunc(teclasNotAscii);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(msec, Timer, 1);

	glutMainLoop();

	return 0;
}

