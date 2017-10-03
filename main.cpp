#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_DEPRECATE
#include<GL\glew.h>
#include<GL\freeglut.h>
#include<stdio.h>
#include <windows.h>
#include <mmsystem.h>

#include<math.h>


 //##############
//STRUTTURE DATI
//##############
typedef struct Coppia {
	float x;
	float y;
} Coppia;

int cluci = 0;
bool flag1 = false;
bool flag2 = false;
bool stop = true;
float fLeft = -3;
float fRight = 3;
float fBottom = -3;
float fTop = 3;
float fNear = 5;
float fFar = 100;

float rotazioneX = 0;
float rotazioneY = 0;

float gx = 0;
float gy = 0;
float gz = 0;

float STEPSTELLA = 10;
float incrementoSTELLA = (2 * M_PI) / STEPSTELLA;
float raggioSTELLA = 1.3;

float camminataZ = 0.0;
float camminataX = 0.0;
float camminataY = 0.0;

int WINDOW_WIDTH = 700;
int WINDOW_HEIGHT = 700;

float mouseXY[] = { 0.5, 0.5 };
float precMouseXY[] = { 0.5, 0.5 };
float originXY[] = { 0.0, 0.0 };

bool flagLeftButton = false;
bool flagRightButton = false;

float passoX = 0.1;
float passoY = 0.1;
float passoZ = 0.1;

GLfloat ambient[4] = { 0.8, 0.8, 0.8, 1 };

GLfloat pos[4] = { 0.0, 0.0, 2.0, 1 };
GLfloat colD[4] = { 0.8, 0.8, 0.8, 1 };
GLfloat colA[4] = { 0.3, 0.3, 0.3, 1 };
GLfloat colS[4] = { 0.0, 0.0, 0.0, 1 };
GLfloat spotDir[3] = { -0.8, 1, 15.0 };

GLfloat matDiffuse[4] = { 0.8, 0.8, 0.8, 1 };
GLfloat matAmbient[4] = { 0.2, 0.2, 0.2, 1 };
GLfloat matSpecular[4] = { 0.0, 0.0, 0.0, 1 };
GLfloat matEmission[4] = { 0.0, 0.0, 0.0, 1 };

GLfloat cpavimento[2][2][3] = { { { -50, -10, 20 }, { 50, -10, 20 }, },
								{ { -50, -10, -200 }, { 50, -10, -200 }, } };

GLfloat cparetesx[2][2][3] = { { { -1, -1, 20 }, { -1, 1, 20 }, },
{ { -1, -1, -30 }, { -1, 1, -30 }, } };

GLfloat cparetedx[2][2][3] = { { { 1, -1, 20 }, { 1, 1, 20 }, },
{ { 1, -1, -30 }, { 1, 1, -30 }, } };

GLfloat csoffitto[2][2][3] = { { { -1, 1, 20 }, { 1, 1, 20 }, },
{ { -1, 1, -30 }, { 1, 1, -30 }, } };

GLuint texnames[3];
char *pavimentoimg = "tronco.rgba";
char *paretiimg = "neve.rgba";
char *luciimg = "oro.rgba";
unsigned * pavimento;
unsigned * pareti;
unsigned *luci;
GLsizei paretiWidth, paretiHeight, pareticomponents;
GLsizei pavimentoWidth, pavimentoHeight, pavimentocomponents;
GLsizei luciWidth, luciHeight, lucicomponents;

static GLfloat sgenparams[] = { 1, 0, 0, 0 };
static GLfloat tgenparams[] = { 0, 1, 0, 0 };
static GLfloat zgenparams[] = { 0, 0, 1, 0 };

//#####################
//PROTOTIPI DI FUNZIONE
//#####################
void myInit();
void myDisplay();
void ramo(float sp, float lung, float trans,float ,int);
void Albero();
void pallina(float lung);
void addobbi();
void sfondo();
void giocoluci(int);
void keyboardCallbackS(int key, int x, int y);
void mouseCallback(int x, int y);
void mouseKeyCallback(int button, int state, int x, int y);
void keyboardCallback(unsigned char key, int x, int y);
GLvoid initTexture();
unsigned * read_texture(char* name, int* width, int* height, int* component);
GLvoid initTexture(unsigned * image, GLsizei imagewidth, GLsizei imageheight);

void getCoordinate(int x, int y);
float * convertCoordinate(int x, int y);
float rad2gr(float rad);
float gr2rad(float gr);
void setVector4f(GLfloat * a, float x1, float x2, float x3, float x4);
void setVector3f(GLfloat * a, float x1, float x2, float x3);

//####
//MAIN
//####
int main(int argv, char** argc){
	glutInit(&argv, argc);
	//cambia il tipo di buffer su cui disegnare
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA);

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Project Natale");
	mciSendString("play C:\\sottofondo.mp3", NULL, 0, NULL);
	mciSendString("stop C:\\sottofondo.mp3", NULL, 0, NULL);
	initTexture();
	myInit();

	glutMouseFunc(mouseKeyCallback);
	glutMotionFunc(mouseCallback);
	glutKeyboardFunc(keyboardCallback);
	glutSpecialFunc(keyboardCallbackS);
	//glutTimerFunc(250, giocoluci, 0);
	glutDisplayFunc(Albero);
	glutMainLoop();

	return 0;
}
void myInit() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
}
void giocoluci(int z) {
	if (!stop) {
		if (!flag1) {
			flag1 = true;
			flag2 = false;
		}
		else {
			flag1 = false;
			flag2 = true;
		}

		glutTimerFunc(250, giocoluci, 0);

		glutPostRedisplay();
	}
}



//#################
//DISPLAY FUNCTIONS
//#################
void ago() {
	glDisable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, texnames[0]);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_S, GL_OBJECT_PLANE, sgenparams);
	glTexGenfv(GL_T, GL_OBJECT_PLANE, zgenparams);
	glEnable(GL_TEXTURE_2D);
	int k = 0;
	glColor3f(0.8,0.5,0.5);
	glutSolidCone(0.1, 2.3, 20, 20);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glPushMatrix();
	
	if (cluci % 2 == 0 && flag1 == true){
		setVector4f(matDiffuse, 1, 1, 0, 1);
		setVector4f(matAmbient, 1, 1, 0, 1);
		setVector4f(matEmission, 0.9, 0.9, 0.7, 1);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);
		if (cluci%4==0)
		glTranslatef(0, 0, 2);
		else
			glTranslatef(0, 0, 1);
		glutSolidSphere(0.08,10,10);
	}
	else if (cluci % 2 == 1 && flag2 == true) {
		setVector4f(matDiffuse, 1, 1, 0, 1);
		setVector4f(matAmbient, 1, 1, 0, 1);
		setVector4f(matEmission, 0.9, 0.9, 0.7, 1);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);
		if (cluci % 4 == 1)
			glTranslatef(0, 0, 2);
		else
			glTranslatef(0, 0, 1);

		glutSolidSphere(0.08, 10, 10);
	}

	cluci++;
	glPopMatrix();

	glPushMatrix();
	

	setVector4f(matDiffuse, 0, 0.3, 0, 1);
	setVector4f(matAmbient, 0, 0.3, 0, 1);
	setVector4f(matEmission, 0, 0, 0, 1);
	setVector4f(matSpecular, 0, 0, 0, 1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);

	for (int grad = 0; grad < 360; grad = grad + (360 / 8)) {
		
		if (k % 2 == 1)
			glTranslatef(0,0,0.15);

		glRotatef(grad, 0, 0, 1);
		for (int i = 0; i < 5; i++) {
			glPushMatrix();
			glColor3f(0, 0.3, 0);
			
			glTranslatef(0, 0, (0.2 + (0.3*i)));
			glRotatef(90, 0, 1, 0);

			glutSolidCylinder(0.025, 0.5, 10, 10);
			glPopMatrix();
			
		}
		k++;
	}
	glPopMatrix();
	
}
void pallina(float lung) {
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, texnames[2]);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_S, GL_OBJECT_PLANE, sgenparams);
	glTexGenfv(GL_T, GL_OBJECT_PLANE, tgenparams);
	glEnable(GL_TEXTURE_2D);
	glColor3f(0.7, 0.7, 0.0);
	glTranslatef(0, -0.6, lung - 0.3);
	glutSolidSphere(0.5, 10, 10);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glPopMatrix();

}
void ramo(float sp,float lung, float trans, float trans2, int nago) {

	

	int k = 0;

	glDisable(GL_LIGHTING);
	glPushMatrix();

	glColor3f(0.8, 0.5, 0.5);
	
	glBindTexture(GL_TEXTURE_2D, texnames[0]);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_S, GL_OBJECT_PLANE, sgenparams);
	glTexGenfv(GL_T, GL_OBJECT_PLANE, tgenparams);
	glEnable(GL_TEXTURE_2D);
	glutSolidCone(sp, lung, 20, 20);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	
	glEnable(GL_LIGHTING);
	
	pallina(lung);

	glPushMatrix();

	if (cluci % 2 == 0 && flag1 == true){
		setVector4f(matDiffuse, 1, 1, 0, 1);
		setVector4f(matAmbient, 1, 1, 0, 1);
		setVector4f(matEmission, 0.9, 0.9, 0.7, 1);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);
		if (cluci % 4 == 0)
			glTranslatef(0, 0, lung-0.5);
		else
			glTranslatef(0, 0, lung-1);
		glutSolidSphere(0.08, 10, 10);
	}
	else if (cluci % 2 == 1 && flag2 == true) {
		setVector4f(matDiffuse, 1, 1, 0, 1);
		setVector4f(matAmbient, 1, 1, 0, 1);
		setVector4f(matEmission, 0.9, 0.9, 0.7, 1);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);
		if (cluci % 4 == 1)
			glTranslatef(0, 0, lung-0.5);
		else
			glTranslatef(0, 0, lung-1);

		glutSolidSphere(0.08, 10, 10);
	}
	
	cluci++;
	glPopMatrix();
	glPushMatrix();
	if (!stop) {
		setVector4f(matDiffuse, 1, 1, 0, 1);
		setVector4f(matAmbient, 1, 1, 0, 1);
		setVector4f(matEmission, 0.9, 0.9, 0.7, 1);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);
		glTranslatef(0, 0, lung);

		glutSolidSphere(0.08, 10, 10);
	}
	glPopMatrix();
	glPushMatrix();
	setVector4f(matDiffuse, 0, 0.3, 0, 1);
	setVector4f(matAmbient, 0, 0.3, 0, 1);
	setVector4f(matEmission, 0, 0, 0, 1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);
	for (int grad = 0; grad < 360; grad = grad + (360 / 8)) {
		if (k % 2 == 1)
			glTranslatef(0, 0, 0.15);
		glRotatef(grad, 0, 0, 1);
		for (int i = 0; i < 5; i++) {
			glPushMatrix();
			glColor3f(0, 0.3, 0);
			glTranslatef(0, 0, (trans + (0.3*i)));
			glRotatef(90, 0, 1, 0);
			glutSolidCylinder(0.025, 0.5, 10, 10);
			glPopMatrix();
		}
		k++;
	}
	glPopMatrix();
	for (int i = 0; i < nago; i++) {
		glPushMatrix();
		glTranslatef(0, 0, trans2+(0.9*i));
		if (i%2==0)
			glRotatef(45, 0, 0, 1);
		else
			glRotatef(-45, 0, 0, 1);

		glRotatef(90, 0, 0, 1);
		glRotatef(90, 0, 1, 0);
		ago();
		glPopMatrix();

	}

	
/*	glPushMatrix();
	ago();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(6, 2.2, 0);
	glRotatef(-45, 1, 0, 0);
	glRotatef(90, 0, 0, 1);
	glRotatef(90, 0, 1, 0);
	glutSolidCone(0.1, 2, 100, 100);
	glPopMatrix();
	*/
}

void addobbi() {

	glPushMatrix();
	glDisable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, texnames[2]);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_S, GL_OBJECT_PLANE, sgenparams);
	glTexGenfv(GL_T, GL_OBJECT_PLANE, tgenparams);
	glEnable(GL_TEXTURE_2D);
	glTranslatef(0, 19.5, 0);
	glRotatef(-19,0,0,1);

	

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.7, 0.7, 0.0);
	glVertex3f(0.0, 0.0,-0.3);
	glColor3f(0.7, 0.7, 0.0);
	int a = 0;
	for (float i = 0; i <= 2 * M_PI; i = i + incrementoSTELLA){

		if (a % 2 == 0)
			glVertex3f(cos(i)*(raggioSTELLA / 5), sin(i)*(raggioSTELLA / 5),0);
		else
			glVertex3f(cos(i)*raggioSTELLA, sin(i)*raggioSTELLA,0);

		a++;
	}
	glVertex3f(1.0*raggioSTELLA / 5, 0.0,0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 19.5, 0);
	glRotatef(-19, 0, 0, 1);
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.7, 0.7, 0.0);
	glVertex3f(0.0, 0.0, 0.3);
	glColor3f(0.7, 0.7, 0.0);
	 a = 0;
	for (float i = 0; i <= 2 * M_PI; i = i + incrementoSTELLA){

		if (a % 2 == 0)
			glVertex3f(cos(i)*(raggioSTELLA / 5), sin(i)*(raggioSTELLA / 5), 0);
		else
			glVertex3f(cos(i)*raggioSTELLA, sin(i)*raggioSTELLA, 0);

		a++;
	}
	glVertex3f(1.0*raggioSTELLA / 5, 0.0, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	setVector4f(matDiffuse, 0.7, 0.7, 0, 1);
	setVector4f(matAmbient, 0.7, 0.7, 0, 1);
	setVector4f(matEmission, 0.9, 0.9, 0.4, 1);
	setVector4f(matSpecular, 0, 0, 0, 1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);

	glTranslatef(0,18.3,0);
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();

	if (cluci % 2 == 0 && flag1 == true){
		setVector4f(matDiffuse, 1, 1, 0, 1);
		setVector4f(matAmbient, 1, 1, 0, 1);
		setVector4f(matEmission, 0.9, 0.9, 0.7, 1);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);
		if (cluci % 4 == 0)
			glTranslatef(0, -0.3,0);
		else
			glTranslatef(0, -0.3, 0);
		glutSolidSphere(0.08, 10, 10);
	}
	else if (cluci % 2 == 1 && flag2 == true) {
		setVector4f(matDiffuse, 1, 1, 0, 1);
		setVector4f(matAmbient, 1, 1, 0, 1);
		setVector4f(matEmission, 0.9, 0.9, 0.7, 1);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);
		if (cluci % 4 == 1)
			glTranslatef(0, -0.3, 0);
		else
			glTranslatef(0, -0.3, 0);

		glutSolidSphere(0.08, 10, 10);
	}

	cluci++;
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
	glRotatef(-90,1,0,0);
	glutSolidCone(0.15, 2, 10, 10);
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
}
void sfondo() {
	setVector4f(matDiffuse, 1, 1, 0, 1);
	setVector4f(matAmbient, 1, 1, 0, 1);
	setVector4f(matEmission, 1, 1, 1, 1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);
	for (float x = fLeft; x < fRight; x = x + 0.5){
		for (float y = fBottom; y < fTop; y = y + 0.5){
			printf("CIAO\n");
			glPushMatrix();
				glTranslatef(x, y, fFar);
				if ((int)x % 2 == 0 && (int)y % 2 == 1 && (int)(x + y) % 2 == 0)
					glutSolidSphere(0.08, 1, 1);
			glPopMatrix();
		}
	}
}
void Albero(){
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(fLeft, fRight, fBottom, fTop, fNear, fFar);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();//SALVO LO STATO CORRENTE PRIMA DI TRASLARE E RUOTARE - M1		
		gluLookAt(0, 0, 30 + camminataZ, 0, 0, camminataZ, 0.0, cos(rotazioneY*M_PI / 180), 0.0);


		glEnable(GL_DEPTH_TEST);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_AUTO_NORMAL);
		glEnable(GL_LIGHTING);
		glPushMatrix();
			glRotatef(gx, 1, 0, 0);
			glRotatef(gy, 0, 1, 0);
			glRotatef(gz, 0, 0, 1);

	
			glPushMatrix();
				glDisable(GL_LIGHTING);
				glBindTexture(GL_TEXTURE_2D, texnames[1]);
				glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
				glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
				glTexGenfv(GL_S, GL_OBJECT_PLANE, sgenparams);
				glTexGenfv(GL_T, GL_OBJECT_PLANE, zgenparams);
				glEnable(GL_TEXTURE_2D);
				glColor3f(1, 1, 1);
				glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 2, 0, 1, 6, 2, &cpavimento[0][0][0]);
				glEnable(GL_MAP2_VERTEX_3);
				glMapGrid2f(200, 0, 1, 200, 0, 1);
				glEvalMesh2(GL_FILL, 0, 200, 0, 200);
				glEnable(GL_LIGHTING);
				glDisable(GL_TEXTURE_2D);
			glPopMatrix();
	
			
			setVector4f(pos, 0, 11, 0, 1);
			setVector4f(colD, 0.8, 0.8, 0.8, 1);
			setVector4f(colA, 0.2, 0.2, 0.2, 1);
			setVector4f(colS, 1, 1, 1, 1);
			setVector3f(spotDir, 0, -1, 0);
			glLightfv(GL_LIGHT1, GL_POSITION, pos);
			glLightfv(GL_LIGHT1, GL_DIFFUSE, colD);
			glLightfv(GL_LIGHT1, GL_SPECULAR, colS);
			glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDir);
			glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60);
			glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0);
			//glEnable(GL_LIGHT1);

			setVector4f(pos, -3, 11, 0, 1);
			setVector4f(colD, 0.8, 0.8, 0.8, 1);
			setVector4f(colA, 0.2, 0.2, 0.2, 1);
			setVector4f(colS, 1, 1, 1, 1);
			setVector3f(spotDir, 0.5, -1, 0);
			glLightfv(GL_LIGHT2, GL_POSITION, pos);
			glLightfv(GL_LIGHT2, GL_DIFFUSE, colD);
			glLightfv(GL_LIGHT2, GL_SPECULAR, colS);
			glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotDir);
			glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 60);
			glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 0);
			glEnable(GL_LIGHT2);

			setVector4f(pos, 3, 11, 0, 1);
			setVector4f(colD, 0.8, 0.8, 0.8, 1);
			setVector4f(colA, 0.2, 0.2, 0.2, 1);
			setVector4f(colS, 1, 1, 1, 1);
			setVector3f(spotDir, -0.5, -1, 0);
			glLightfv(GL_LIGHT3, GL_POSITION, pos);
			glLightfv(GL_LIGHT3, GL_DIFFUSE, colD);
			glLightfv(GL_LIGHT3, GL_SPECULAR, colS);
			glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spotDir);
			glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 60);
			glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 0);
			glEnable(GL_LIGHT3);

	

			glTranslatef(0, -10, 0);

			glPushMatrix();
			glDisable(GL_LIGHTING);
			glBindTexture(GL_TEXTURE_2D, texnames[0]);
			glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
			glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
			glTexGenfv(GL_S, GL_OBJECT_PLANE, sgenparams);
			glTexGenfv(GL_T, GL_OBJECT_PLANE, zgenparams);
			glEnable(GL_TEXTURE_2D);
			glColor3f(0.8, 0.5, 0.5);
			glRotatef(270, 1, 0, 0);
			glutSolidCone(0.5,20,50,50);
			glEnable(GL_LIGHTING);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0, 2.2, 0);
			for (int grad = 0; grad < 360; grad = grad + 30){
				glPushMatrix();
				glRotatef(grad, 0, 1, 0);
				ramo(0.25,9,8.2,3,6);
				glPopMatrix();
			}
			glPopMatrix();
	
	
			glPushMatrix();
			glTranslatef(0, 4.4, 0);
			for (int grad = 20; grad < 360; grad = grad + 36){
				glPushMatrix();
				glRotatef(grad, 0, 1, 0);
				ramo(0.23, 8.5, 6.7, 3.5, 4);
				glPopMatrix();
			}
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0, 6.6, 0);
			for (int grad = 0; grad < 360; grad = grad + 45){
				glPushMatrix();
				glRotatef(grad, 0, 1, 0);	
				ramo(0.2, 7, 5.2,3,3);
				glPopMatrix();
			}
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0, 8.8, 0);
			for (int grad = 20; grad < 360; grad = grad + 52){
				glPushMatrix();
				glRotatef(grad, 0, 1, 0);
				ramo(0.17, 5.5, 3.7, 1.6, 3);
				glPopMatrix();
			}
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0, 11, 0);
			for (int grad = 0; grad < 360; grad = grad + 60){
				glPushMatrix();
				glRotatef(grad, 0, 1, 0);
				ramo(0.15, 4, 1.9, 0.8,2);
				glPopMatrix();
			}
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0, 13.2, 0);
			for (int grad = 20; grad < 360; grad = grad + 72){
				glPushMatrix();
				glRotatef(grad, 0, 1, 0);
				ramo(0.12, 3, 1, 0.2, 2);
				glPopMatrix();
			}
			glPopMatrix();

			glPushMatrix();
				glTranslatef(0, 15.4, 0);
				for (int grad = 0; grad < 360; grad = grad + 72){
					glPushMatrix();
					glRotatef(grad, 0, 1, 0);
					ramo(0.1, 2, 0.5, 0.2, 1);
					glPopMatrix();
				}
			glPopMatrix();

			glPushMatrix();
				glTranslatef(0, 17.6, 0);
				for (int grad = 20; grad < 360; grad = grad + 72){
					glPushMatrix();
						glRotatef(grad, 0, 1, 0);
						ramo(0.08, 1, 0, 0.2, 0);
					glPopMatrix();
				}
			glPopMatrix();

			addobbi();
			//sfondo();

		glPopMatrix();

	glPopMatrix();

	glFlush();
	glDisable(GL_TEXTURE_2D);
	glutSwapBuffers();
}

void myDisplay(){
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(fLeft, fRight, fBottom, fTop, fNear, fFar);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();//SALVO LO STATO CORRENTE PRIMA DI TRASLARE E RUOTARE - M1		
	gluLookAt(0, 0, 30 + camminataZ, 0, 0, camminataZ, 0.0, cos(rotazioneY*M_PI / 180), 0.0);


	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_AUTO_NORMAL);
	//glFrontFace(GL_CCW);       // Counterclockwise polygons face out
	//glEnable(GL_CULL_FACE);    // Do not try to display the back sides
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

	//glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);


	glPushMatrix();
	glRotatef(gx, 1, 0, 0);
	glRotatef(gy, 0, 1, 0);
	glRotatef(gz, 0, 0, 1);

	//##########
	//SETUP LUCI
	//##########
	setVector4f(pos, 0, 0.85, 0, 1);
	setVector4f(colD, 0.8, 0.8, 0.8, 1);
	setVector4f(colA, 0.2, 0.2, 0.2, 1);
	setVector4f(colS, 1, 1, 1, 1);
	setVector3f(spotDir, 0, -1, 0);
	glLightfv(GL_LIGHT1, GL_POSITION, pos);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, colD);
	glLightfv(GL_LIGHT1, GL_AMBIENT, colA);
	glLightfv(GL_LIGHT1, GL_SPECULAR, colS);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDir);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0);
	glEnable(GL_LIGHT1);

	setVector4f(pos, -0.9, 0.6, 15.0, 1);
	setVector4f(colD, 0.8, 0.8, 0.8, 1);
	setVector4f(colA, 0.4, 0.4, 0.4, 1);
	setVector4f(colS, 1, 1, 1, 1);
	setVector3f(spotDir, 0, 1, 0);
	glLightfv(GL_LIGHT2, GL_POSITION, pos);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, colD);
	glLightfv(GL_LIGHT2, GL_AMBIENT, colA);
	glLightfv(GL_LIGHT2, GL_SPECULAR, colS);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotDir);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 5);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 60);
	glEnable(GL_LIGHT2);

	setVector4f(pos, 0.9, 0.6, 15.0, 1);
	setVector4f(colD, 0.8, 0.8, 0.8, 1);
	setVector4f(colA, 0.4, 0.4, 0.4, 1);
	setVector4f(colS, 1, 1, 1, 1);
	setVector3f(spotDir, 0, 1, 0);
	glLightfv(GL_LIGHT3, GL_POSITION, pos);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, colD);
	glLightfv(GL_LIGHT3, GL_AMBIENT, colA);
	glLightfv(GL_LIGHT3, GL_SPECULAR, colS);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spotDir);
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 10);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 60);
	glEnable(GL_LIGHT3);

	setVector4f(pos, -0.9, 0.5, 5.0, 1);
	setVector4f(colD, 0.8, 0.8, 0.8, 1);
	setVector4f(colA, 0.2, 0.2, 0.2, 1);
	setVector4f(colS, 0.5, 0.5, 0.5, 1);
	setVector3f(spotDir, 0, 1, 0);
	glLightfv(GL_LIGHT4, GL_POSITION, pos);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, colD);
	glLightfv(GL_LIGHT4, GL_AMBIENT, colA);
	glLightfv(GL_LIGHT4, GL_SPECULAR, colS);
	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, spotDir);
	glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 10);
	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 60);
	glEnable(GL_LIGHT4);

	setVector4f(pos, 0.9, 0.5, 5.0, 1);
	setVector4f(colD, 0.8, 0.8, 0.8, 1);
	setVector4f(colA, 0.2, 0.2, 0.2, 1);
	setVector4f(colS, 0.5, 0.5, 0.5, 1);
	setVector3f(spotDir, 0, 1, 0);
	glLightfv(GL_LIGHT5, GL_POSITION, pos);
	glLightfv(GL_LIGHT5, GL_DIFFUSE, colD);
	glLightfv(GL_LIGHT5, GL_AMBIENT, colA);
	glLightfv(GL_LIGHT5, GL_SPECULAR, colS);
	glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, spotDir);
	glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 10);
	glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 60);
	glEnable(GL_LIGHT5);

	setVector4f(pos, -0.9, 0.5, -5.0, 1);
	setVector4f(colD, 0.8, 0.8, 0.8, 1);
	setVector4f(colA, 0.2, 0.2, 0.2, 1);
	setVector4f(colS, 0.5, 0.5, 0.5, 1);
	setVector3f(spotDir, 0, 1, 0);
	glLightfv(GL_LIGHT6, GL_POSITION, pos);
	glLightfv(GL_LIGHT6, GL_DIFFUSE, colD);
	glLightfv(GL_LIGHT6, GL_AMBIENT, colA);
	glLightfv(GL_LIGHT6, GL_SPECULAR, colS);
	glLightfv(GL_LIGHT6, GL_SPOT_DIRECTION, spotDir);
	glLightf(GL_LIGHT6, GL_SPOT_EXPONENT, 10);
	glLightf(GL_LIGHT6, GL_SPOT_CUTOFF, 60);
	glEnable(GL_LIGHT6);

	setVector4f(pos, 0.9, 0.5, -5.0, 1);
	setVector4f(colD, 0.8, 0.8, 0.8, 1);
	setVector4f(colA, 0.2, 0.2, 0.2, 1);
	setVector4f(colS, 0.5, 0.5, 0.5, 1);
	setVector3f(spotDir, 0, 1, 0);
	glLightfv(GL_LIGHT7, GL_POSITION, pos);
	glLightfv(GL_LIGHT7, GL_DIFFUSE, colD);
	glLightfv(GL_LIGHT7, GL_AMBIENT, colA);
	glLightfv(GL_LIGHT7, GL_SPECULAR, colS);
	glLightfv(GL_LIGHT7, GL_SPOT_DIRECTION, spotDir);
	glLightf(GL_LIGHT7, GL_SPOT_EXPONENT, 10);
	glLightf(GL_LIGHT7, GL_SPOT_CUTOFF, 60);
	glEnable(GL_LIGHT7);

	setVector4f(matDiffuse, 0.8, 0.8, 0.8, 1);
	setVector4f(matAmbient, 0.4, 0.4, 0.4, 1);
	setVector4f(matEmission, 0.5, 0.5, 0.5, 1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);

	glBindTexture(GL_TEXTURE_2D, texnames[2]);
	glPushMatrix();
	glTranslatef(0, 0.9, 0);
	glRotatef(-90, 1, 0, 0);
	glutSolidCone(0.15, 0.1, 100, 100);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(1, 0.5, 15);
	glRotatef(90, 1, 0, 0);
	glutSolidCone(0.1, 0.2, 100, 100);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-1, 0.5, 15);
	glRotatef(90, 1, 0, 0);
	glutSolidCone(0.1, 0.2, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1, 0.5, 5);
	glRotatef(90, 1, 0, 0);
	glutSolidCone(0.1, 0.2, 100, 100);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-1, 0.5, 5);
	glRotatef(90, 1, 0, 0);
	glutSolidCone(0.1, 0.2, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1, 0.5, -5);
	glRotatef(90, 1, 0, 0);
	glutSolidCone(0.1, 0.2, 100, 100);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-1, 0.5, -5);
	glRotatef(90, 1, 0, 0);
	glutSolidCone(0.1, 0.2, 100, 100);
	glPopMatrix();


	setVector4f(matDiffuse, 0.8, 0.8, 0.8, 1);
	setVector4f(matAmbient, 0.5, 0.5, 0.5, 1);
	setVector4f(matEmission, 0.0, 0.0, 0.0, 1);
	setVector4f(matSpecular, 0.0, 0.0, 0.0, 1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);

	
	
	glDisable(GL_TEXTURE_2D);
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 2, 0, 1, 6, 2, &csoffitto[0][0][0]);
	glEnable(GL_MAP2_VERTEX_3);
	glMapGrid2f(200, 0, 1, 200, 0, 1);
	glEvalMesh2(GL_FILL, 0, 200, 0, 200);

	setVector4f(matDiffuse, 0.7, 0.5, 0.5, 1);
	setVector4f(matAmbient, 0.4, 0.4, 0.3, 1);
	setVector4f(matEmission, 0.0, 0.0, 0.0, 1);
	setVector4f(matSpecular, 0.0, 0.0, 0.0, 1);
	glEnable(GL_TEXTURE_2D);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);
	
	
	glBindTexture(GL_TEXTURE_2D, texnames[1]);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_S, GL_OBJECT_PLANE, zgenparams);
	glTexGenfv(GL_T, GL_OBJECT_PLANE, tgenparams);
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 2, 0, 1, 6, 2, &cparetesx[0][0][0]);
	glEnable(GL_MAP2_VERTEX_3);
	glMapGrid2f(200, 0, 1, 200, 0, 1);
	glEvalMesh2(GL_FILL, 0, 200, 0, 200);
	
	glBindTexture(GL_TEXTURE_2D, texnames[1]);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_S, GL_OBJECT_PLANE, zgenparams);
	glTexGenfv(GL_T, GL_OBJECT_PLANE, tgenparams);
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 2, 0, 1, 6, 2, &cparetedx[0][0][0]);
	glEnable(GL_MAP2_VERTEX_3);
	glMapGrid2f(200, 0, 1, 200, 0, 1);
	glEvalMesh2(GL_FILL, 0, 200, 0, 200);

	setVector4f(matDiffuse, 0.0, 0.0, 0.8, 1);
	setVector4f(matAmbient, 0.0, 0.0, 0.4, 1);
	setVector4f(matEmission, 0.0, 0.0, 0.0, 1);
	setVector4f(matSpecular, 1, 1, 1, 1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128);

	glBindTexture(GL_TEXTURE_2D, texnames[0]);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_S, GL_OBJECT_PLANE, sgenparams);
	glTexGenfv(GL_T, GL_OBJECT_PLANE, zgenparams);
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 2, 0, 1, 6, 2, &cpavimento[0][0][0]);
	glEnable(GL_MAP2_VERTEX_3);
	glMapGrid2f(200, 0, 1, 200, 0, 1);
	glEvalMesh2(GL_FILL, 0, 200, 0, 200);

	

	glPopMatrix();

	glPopMatrix();//RIPRISTINO LO STATO M1 (gluLookAt)
	glFlush();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHT2);
	glDisable(GL_LIGHT3);
	glDisable(GL_LIGHT4);
	glDisable(GL_LIGHT5);
	glDisable(GL_LIGHT6);
	glDisable(GL_LIGHT7);
	glutSwapBuffers();//GESTIONE DEL DOPPIO BUFFER
}






//##################
//CALLBACK FUNCTIONS
//##################


//MOUSE MOTION CALLBACK
void mouseCallback(int x, int y) {
	//printf("\n x=%d y=%d", x, y);
	getCoordinate(x, y);//salva le coordinate nelle variabili globali, aggiornando prec

	float diff, diffX, diffY;
	if (flagLeftButton) {
		diffX = mouseXY[0] - precMouseXY[0];
		diffY = mouseXY[1] - precMouseXY[1];
		rotazioneX = rotazioneX + diffY;
		rotazioneY = rotazioneY + diffX;

	}
	glutPostRedisplay();
}



//MOUSE PRESSED CALLBACK
void mouseKeyCallback(int button, int state, int x, int y) {
	precMouseXY[0] = convertCoordinate(x, y)[0];
	precMouseXY[1] = convertCoordinate(x, y)[1];
	mouseXY[0] = convertCoordinate(x, y)[0];
	mouseXY[1] = convertCoordinate(x, y)[1];

	float diff;
	if (state == GLUT_DOWN) {
		switch (button)
		{
		case GLUT_LEFT_BUTTON:
			flagLeftButton = true;
			break;
		case GLUT_RIGHT_BUTTON:
			flagRightButton = true;
			break;
		default:
			break;
		}
	}

	if (state == GLUT_UP) {
		flagLeftButton = false;
		flagRightButton = false;
	}

}

//KEYBOARD CALLBACK
void keyboardCallback(unsigned char key, int x, int y) {
	switch (key){
	case 'w':
		camminataZ = camminataZ - 0.2;
		printf("camminataZ: %f\n", camminataZ);
		break;
	case 's':
		camminataZ = camminataZ + 0.2;
		printf("camminataZ: %f\n", camminataZ);
		break;
	case 'l':
		if (stop){
			stop = false;
			mciSendString("play C:\\sottofondo.mp3", NULL, 0, NULL);
			giocoluci(0);
			glEnable(GL_LIGHT1);
		}
		else {
			stop = true;
			flag1 = false;
			flag2 = false;
			mciSendString("stop C:\\sottofondo.mp3", NULL, 0, NULL);
			glDisable(GL_LIGHT1);

		}
	default:
		break;
	}
	glutPostRedisplay();
}

void keyboardCallbackS(int key, int x, int y) {

	switch (key)
	{
	case GLUT_KEY_UP:
		gx += 2;
		break;
	case GLUT_KEY_LEFT:
		gy -= 2;
		break;
	case GLUT_KEY_RIGHT:
		gy += 2;
		break;
	case GLUT_KEY_DOWN:
		gx -= 2;
		break;
	case GLUT_KEY_ALT_R:
		gz -= 2;
		break;
	case GLUT_KEY_CTRL_R:
		gz += 2;
	default:
		break;
	}
	glutPostRedisplay();
}




//#################
//UTILITY FUNCTIONS
//#################

float gr2rad(float gr) {
	return (gr*M_PI / 180);
}

float rad2gr(float rad) {
	return ((360 * rad) / (2 * M_PI));
}


void getCoordinate(int x, int y) {
	precMouseXY[0] = mouseXY[0];
	precMouseXY[1] = mouseXY[1];

	mouseXY[0] = convertCoordinate(x, y)[0];
	mouseXY[1] = convertCoordinate(x, y)[1];

}

float * convertCoordinate(int x, int y) {

	float XY[2];
	XY[0] = ((float)x / (WINDOW_WIDTH / 2) - 1);
	XY[1] = -((float)y / (WINDOW_HEIGHT / 2) - 1);
	return XY;
	//printf("\npmx=%f pmy=%f \n mx=%f my=%f",precMouseXY[0], precMouseXY[1],mouseXY[0], mouseXY[1]);
}


void setVector4f(GLfloat * a, float x1, float x2, float x3, float x4) {
	a[0] = x1;
	a[1] = x2;
	a[2] = x3;
	a[3] = x4;
}


void setVector3f(GLfloat * a, float x1, float x2, float x3) {
	a[0] = x1;
	a[1] = x2;
	a[2] = x3;
}
GLvoid initTexture() {
	GLsizei sWidth = 256, sHeight = 256;
	
	unsigned * sIPav, * sIPar, *sILuci;

	pavimento = read_texture(pavimentoimg, &pavimentoWidth, &pavimentoHeight, &pavimentocomponents);
	pareti = read_texture(paretiimg, &paretiWidth, &paretiHeight, &pareticomponents);
	luci = read_texture(luciimg, &luciWidth, &luciHeight, &lucicomponents);

	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	sIPav = (unsigned*)malloc(sWidth*sHeight * 4 * sizeof(unsigned));
	sIPar = (unsigned*)malloc(sWidth*sHeight * 4 * sizeof(unsigned));
	sILuci = (unsigned*)malloc(sWidth*sHeight * 4 * sizeof(unsigned));
	gluScaleImage(GL_RGBA, pavimentoWidth, pavimentoHeight, GL_UNSIGNED_BYTE, pavimento, sWidth, sHeight, GL_UNSIGNED_BYTE, sIPav);
	gluScaleImage(GL_RGBA, paretiWidth, paretiHeight, GL_UNSIGNED_BYTE, pareti, sWidth, sHeight, GL_UNSIGNED_BYTE, sIPar);
	gluScaleImage(GL_RGBA, luciWidth, luciHeight, GL_UNSIGNED_BYTE, luci, sWidth, sHeight, GL_UNSIGNED_BYTE, sILuci);

	glGenTextures(3, texnames);
	glBindTexture(GL_TEXTURE_2D, texnames[0]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sWidth, sHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, sIPav);

	
	glBindTexture(GL_TEXTURE_2D, texnames[1]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sWidth, sHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, sIPar);

	
	glBindTexture(GL_TEXTURE_2D, texnames[2]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sWidth, sHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, sILuci);

	glEnable(GL_TEXTURE_2D);
}