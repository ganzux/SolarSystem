#include<windows.h>
#include <stdlib.h>
#include<time.h>

#ifdef __APPLE__
       #include <GLUT/glut.h>
#else
     #include <GL/glut.h>
#endif



/***************************************
      *     Inicializacion de variables     *
      ***************************************/

      GLfloat rotA=0,rot1=0,rot2=0,rot3=0,rot4=0,rot5=0,rot6=0,rot7=0,rot8=0;
      GLfloat rdS=30,
        rd1=6,
        rd2=12,
        rd3=9,
        rd4=6,
        rd5=18,
        rd6=15,
        rd7=12,
        rd8=12;
      GLfloat posx=0,posz=0;

     /*******************************************************
      *     Inicializacion de variables para iluminacion    *
      *******************************************************/

      GLfloat mat_diffuse [] = {0.0, 0.0, 0.0, 1.0};
      GLfloat mat_specular [] = {0.5, 0.5, 0.5, 1.0};
      GLfloat mat_shininess [] = {50};
      GLfloat mat_emission [] = {0.5, 0.2, 0.2, 1.0};

/**********************************************************************/
/**********************************************************************/

void resize(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);//matriz de proyeccion
    glLoadIdentity();//cargamos la matriz identidad
    glOrtho(-300,300,-300,300,-300,300);
    glMatrixMode(GL_MODELVIEW);//matriz de modelado
    glLoadIdentity();//matriz identidad
    glEnable(GL_DEPTH_TEST);//activamos el Test de profundidad
    glDepthFunc(GL_LEQUAL);//funcion de borrrado buffer depth

}

     /***********************
      *      iluminacion    *
      ***********************/

void iluminacion () {
GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};
glLightfv(GL_LIGHT0,GL_POSITION, light_position);

glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
glEnable (GL_LIGHTING);
glEnable (GL_LIGHT0);
glEnable(GL_COLOR_MATERIAL);

}
     /****************************
      *   EJES DE COORDENADAS    *
      ****************************/


void ejes() {
       glColor3f(1,1,1);
       glBegin(GL_LINES);
            glVertex3f(-200,0,0);
            glVertex3f(200,0,0);
            glVertex3f(0,-200,0);
            glVertex3f(0,200,0);
            glVertex3f(0,0,-200);
            glVertex3f(0,0,200);

       glEnd();

}

    /************************************
     *     CREACION DE LOS PLANETAS     *
     ************************************/
void anillos() {
     glColor3f(1,0.6,0);
     glRotatef(rotA,1,0,1);
     glutSolidTorus(3, 20, 10,50);
}

void sol() {
     glColor3f(1.0,1.0,0.0);
     glutSolidSphere(rdS,60,60);
}

void mercurio() {
     glColor3f(0.4,0.4,0.4);
     glutSolidSphere(rd1,60,60);
}

void venus() {
     glColor3f(1.0,0.2,0.0);
     glutSolidSphere(rd2,60,60);
}

void tierra() {
     glColor3f(0.0,0.4,1.0);
     glutSolidSphere(rd3,60,60);
}

void marte() {
     glColor3f(0.2,0.4,0.3);
     glutSolidSphere(rd4,60,60);
}

void jupiter() {
     glColor3f(0.2,0.2,0.2);
     glutSolidSphere(rd5,60,60);
}

void saturno() {
     glColor3f(1.0,0.4,0.0);
     glutSolidSphere(rd6,60,60);
     anillos();
}

void urano() {
     glColor3f(0.2,1.0,0.2);
     glutSolidSphere(rd7,60,60);
}

void neptuno() {
     glColor3f(0.0,0.0,1.0);
     glutSolidSphere(rd8,60,60);
}

void estrellas() {
    int i,j,k; srand(time(NULL));
    int h=0;
         glBegin(GL_POINTS);
         while(h<100){
         i=(rand()%300);
         j=(rand()%300);
         k=(rand()%300);
           glColor3f(1,1,1);
           glVertex3f(i,j,k);
           glVertex3f(i,-j,k);
           glColor3f(0,1,0);
           glVertex3f(-i,-j,k);
           glColor3f(0,0,1);
           glVertex3f(-i,j,-k);
           h++;}
    glEnd();

}

void paintPlanet(GLfloat rotacion, int x, int y, int z){
     glPopMatrix();
     glPushMatrix();
     glRotatef(rotacion,0,1,0);
     glTranslatef(x,y,z);
}

void display() {
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     glLoadIdentity();
     gluLookAt(1.0,1.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0);
     iluminacion ();
     estrellas();
     ejes();
     sol();

     paintPlanet(rot1, -40, 0, 0);
     mercurio();

     paintPlanet(rot2,  60, 0, 0);
     venus();

     paintPlanet(rot3, 0, 0, -80);
     tierra();

     paintPlanet(rot4, 100, 0, 0);
     marte();

     paintPlanet(rot5, -120, 0, 0);
     jupiter();

     paintPlanet(rot6, 0, 0, 160);
     saturno();

     paintPlanet(rot7, -170, 0, 0);
     urano();

     paintPlanet(rot8, 190, 0, 0);
     neptuno();
     glPopMatrix();

     rotA=rotA+0.03;
     rot1=rot1+0.1;
     rot2=rot2+0.015;
     rot3=rot3+0.013;
     rot4=rot4+0.05;
     rot5=rot5+0.04;
     rot6=rot6+0.03;
     rot7=rot7+0.02;
     rot8=rot8+0.01;

     glutSwapBuffers();
}

void key(unsigned char key, int x, int y) {
    switch(key) {
          case 27 :
            exit(0);
            break;
        default:
            break;
    }

    glutPostRedisplay();
}


void idle() {
    glutPostRedisplay();//redibuja el display
}


int main (int argc, char *argv[]) {
    glutInit(&argc, argv);//inicializacion del glut
    glutInitWindowSize(640,480);//tamaño de la ventana
    glutInitWindowPosition(400,100);//posicion de la ventana en el monitor
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);//modo de visualizacion inicial
    glutCreateWindow("URJC Solar System");//nombre de la ventana
    glClearColor(0,0,0,0);//especificar valores claros para el color del buffer
    glutReshapeFunc(resize);//control de ventanas
    glutDisplayFunc(display);//establece la devolución de llamada de visualización de la ventana actual
    glutKeyboardFunc(key);//funcion teclado especial
    glutFullScreen();
    glutIdleFunc(idle);//funcion de animacion

   // char soundfile[20] ="C:\planeta.wav";//sonido
    //PlaySound((LPCSTR)soundfile, NULL, SND_FILENAME | SND_ASYNC );//sonido

    glutMainLoop();//bucle de procesamiento de eventos de GLUT
    return 0;
}

