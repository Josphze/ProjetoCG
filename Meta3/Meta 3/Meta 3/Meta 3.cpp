#include <windows.h>			
#include <stdio.h>				
#include <fstream>				

#include <string>
#include <fstream>

#include <errno.h>
#include <GL\glew.h>			
#include <GL\freeGlut.h>		

#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"opengl32.lib")

GLint	wScreen = 1000, hScreen = 1000;	
GLint  uniOp;
GLint  uniDir;
GLint  uniVar;

float  Direcao[] = { 0, 1, 0 };
float  opcao = -45;
float gouraud = 0;

char filenameV[] = "Vshader.txt";
char filenameF[] = "Fshader.txt";
char* VertexShaderSource;
char* FragmentShaderSource;

GLuint  VertexShader, FragmentShader;
GLuint  ShaderProgram;


char* readShaderFile(char* FileName) 
{
	char* DATA = NULL;
	int   flength = 0;
	FILE* filepoint;
	errno_t err;

	err = fopen_s(&filepoint, FileName, "r");
	if (!err) {
		fseek(filepoint, 0, SEEK_END);
		flength = ftell(filepoint);
		rewind(filepoint);

		DATA = (char*)malloc(sizeof(char) * (flength + 1));
		flength = fread(DATA, sizeof(char), flength, filepoint);

		DATA[flength] = '\0';
		fclose(filepoint);
		return DATA;
	}
	else {
		printf(" --------  Error while reading  %s ", FileName);
	}
}


void BuiltShader(void) 
{
	GLEW_ARB_vertex_shader;
	GLEW_ARB_fragment_shader;

	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	VertexShaderSource = readShaderFile(filenameV);
	FragmentShaderSource = readShaderFile(filenameF);

	const char* VS = VertexShaderSource;
	const char* FS = FragmentShaderSource;
	glShaderSource(VertexShader, 1, &VS, NULL);
	glShaderSource(FragmentShader, 1, &FS, NULL);
	free(VertexShaderSource);
	free(FragmentShaderSource);

	glCompileShaderARB(VertexShader);
	glCompileShaderARB(FragmentShader);

	ShaderProgram = glCreateProgramObjectARB();
	glAttachShader(ShaderProgram, VertexShader);
	glAttachShader(ShaderProgram, FragmentShader);
	glLinkProgram(ShaderProgram);

	glUseProgramObjectARB(ShaderProgram);
}


void InitShader(void) 
{
	BuiltShader();

	uniDir = glGetUniformLocation(ShaderProgram, "Direcao");
	glUniform3fv(uniDir, 1, Direcao);
	uniVar = glGetUniformLocation(ShaderProgram, "gouraud");
	glUniform1f(uniVar, gouraud);
}


void DeInitShader(void) 
{
	glDetachShader(ShaderProgram, VertexShader);
	glDetachShader(ShaderProgram, FragmentShader);
	glDeleteShader(ShaderProgram);
}


void Inicializa(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);	

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
}

void Desenha(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, wScreen, hScreen);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(80, 1, 0.1, 10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 1, 10, 0, 0, 0, 0, 1, 0);

	gouraud = 0;

	glUniform1f(uniVar, gouraud);
	glUniform3fv(uniDir, 1, Direcao);

	Direcao[0] = cos(3.14 * opcao / 180.0);
	Direcao[2] = sin(3.14 * opcao / 180.0);

	glColor3f(0.5, 0, 0.5);
	glPushMatrix();
		glTranslatef(-5, 0, 0);
		glutSolidSphere(2, 20, 20);
	glPopMatrix();

	gouraud = 1;

	glUniform1f(uniVar, gouraud);
	glUniform3fv(uniDir, 1, Direcao);

	glColor3f(0.2, 0.6, 0);
	glPushMatrix();
		glTranslatef(5, 0, 0);
		glutSolidSphere(2, 20, 20);
	glPopMatrix();

	glutSwapBuffers();						
}


void Teclado(unsigned char key, int x, int y) {

	switch (key) {

	case 'o':	case 'O':
		opcao = opcao + 10;
		glutPostRedisplay();
		break;

	case 27:					
		exit(0);
		break;
	}
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 1000);					
	glutInitWindowPosition(0, 0);					
	glutCreateWindow("Rodar Luz: O");

	Inicializa();

	GLenum err = glewInit();
	InitShader();

	DeInitShader();

	glutDisplayFunc(Desenha);						
	glutKeyboardFunc(Teclado);						
	glutIdleFunc(Desenha);

	glutMainLoop();							
	return 1;
}
