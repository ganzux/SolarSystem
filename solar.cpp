#include <gl\gl.h>
#include <gl\glu.h>
#include <windows.h>
#include <stdlib.h>
#include<time.h>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <string>



// Functions
void myinit(void);
void drawPlane(void);
void dayAdd (void);
void daySubtract (void);
void yearAdd (void);
void yearSubtract (void);
void display(void);
void myReshape(int w, int h);
void printString(char *s);
void coords();
void printPlanet(int textureIdx, float r, float g, float b, float radix);
GLuint LoadTexture( const char * filename );
void KeyBoardControl(int key,int x,int y );
static void idle(void);
void mouse_movement (int x, int y);
void drawSaturnRing();


// Variables
static float velocity = 25;
static bool animate = true;
static int slices = 128;
static int stacks = 128;
static float zoom_factor = 0.3;
static float prsc = -20;

static float year = 0;
static float day = 0;

// 360 degrees divided by 28 days (a complete traslation)
static float moonMovementByDay  = 360 / 28;

// 360 degrees divided by 28 (28 days a complete traslation)
// divided by 24 (hours by day)
static float moonMovementByHour = moonMovementByDay / 24;

// In 1 day, our planet moves 360 degrees divided by 365 days by year
static float planetMovementByDay = (float)360/(float)365;

// In 1 hour, our planet moves 360 degrees divided by 365 days by year
// divided by 24 (hours by day)
static float planetMovementByHour = planetMovementByDay/(float)24;

// Lighting values
GLfloat  whiteLight[] = { 0.1, 0.2, 0.3, 1 };
GLfloat  sourceLight[] = { 0.9f, 0.9f, 0.9f, 1.0f };
GLfloat	 lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };

// Texture prechargued for fast rendered
GLuint textures[12];
bool texturesLoaded = false;

GLsizei current_w, current_h;

// Planet Radix
GLfloat rdS=0.9,
        rdBig=0.5,
        rdSmall=0.3,
        rdMoon=0.1;

// Initial position
GLfloat sunX = 0,
        sunY = -2,
        sunZ = -20;


// Main program
// Open the initial window with a title bar and window node RGBA. Input events
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (800, 600);
    glutInitWindowPosition(0, 0);
    glutCreateWindow ("My Personal Solar System");

    // Initial configuration
    myinit();

    glutSpecialFunc(KeyBoardControl);
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutMotionFunc(mouse_movement);  // Not tested

    glutMainLoop();
    return(0);
}

void KeyBoardControl(int key,int x,int y ) {

    switch (key) {
        // UP: +1 hour
        case GLUT_KEY_UP:
             /*day += (moonMovementByHour*velocity);
             year += (planetMovementByHour*velocity);*/
             sunY += 0.1;
        break;

        // DOWN: -1 hour
        case GLUT_KEY_DOWN:
             /*day -= (moonMovementByHour*velocity);
             year -= (planetMovementByHour*velocity);*/
             sunY -= 0.1;
        break;

        // LEFT: -1 day
        case GLUT_KEY_LEFT:
             /*year -= (planetMovementByDay*velocity);
             day -= (planetMovementByHour*velocity);*/
             sunX -= 0.1;
        break;

        // RIGHT: +1 day
        case GLUT_KEY_RIGHT:
             /*year += (planetMovementByDay*velocity);
             day += (planetMovementByHour*velocity);*/
             sunX += 0.1;
        break;

        case GLUT_KEY_F1:
             velocity ++;
        break;

        case GLUT_KEY_F2:
              velocity --;
              velocity = (velocity < 1) ? 1 : velocity;
        break;

        // Reset init values
        case GLUT_KEY_F3:
             year = 0;
             day = 0;
             velocity = 1;
        break;

        // Automatic animation
        case GLUT_KEY_F4:
             animate = !animate;
        break;
        
        // + zoom
        case GLUT_KEY_F5:
             if (zoom_factor<2) {
                 zoom_factor += 0.1;
             }
             myReshape( current_w, current_h);
        break;
        
        // Automatic animation
        case GLUT_KEY_F6:
             if (zoom_factor>0.1) {
                 zoom_factor -= 0.1;
             }
             myReshape( current_w, current_h);
        break;

        case GLUT_KEY_F11:
            if (slices>3 && stacks>3) {
                slices--;
                stacks--;
            }
        break;

        case GLUT_KEY_F12:
            slices++;
            stacks++;
        break;
        
        case GLUT_KEY_PAGE_UP:
             sunZ++;
        break;
        case GLUT_KEY_PAGE_DOWN:
             sunZ--;
        break;
    }
    glutPostRedisplay();
}




void display(void) {
     
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();
   glTranslatef(sunX, sunY, sunZ);	
   glPushMatrix();

   coords();

   // SUN
   glDisable(GL_LIGHTING);
   glRotatef(year, 1, 0, 0); //rotates Sun along X axis
   glRotatef(year, 0, 1, 0); //rotates Sun along Y axis
   glRotatef(year, 0, 0, 1); //rotates Sun along Z axis

   printPlanet(0, rdS, 1.0, 1.0, 1.0);
   glDisable(GL_TEXTURE_2D);
   glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
   glEnable(GL_LIGHTING);
   glPopMatrix();

   // MERCURY
   glPushMatrix();
   glRotatef (year*6.3, 0, 1, 0);   // 58 days
   glTranslatef (2, 0.0, 0.0);
   glRotatef (day*58, 0.0, 1.0, 0.0);
   glRotatef (90.0, 1.0, 0.0, 0.0);
   printPlanet(1, rdSmall, 1.0, 1.0, 1.0);
   glPopMatrix();

   
   // VENUS
   glPushMatrix();
   glRotatef (year*1.63, 0, 1, 0);      // 224 days
   glTranslatef (3, 0.0, 0.0);
   glRotatef (day*243, 0.0, 1.0, 0.0);
   glRotatef (177.30, 1.0, 0.0, 0.0);
   printPlanet(2, rdSmall, 1.0, 1.0, 1.0);
   glPopMatrix();
   
   // EARTH
   glPushMatrix();
   glRotatef (year, 0, 1, 0);
   glTranslatef (5, 0.0, 0.0);
   glRotatef (day, 0.0, 1.0, 0.0);
   glRotatef (23.26, 1.0, 0.0, 0.0);
   printPlanet(3, rdSmall, 1.0, 1.0, 1.0);

   // MOON
   glRotatef (day, (GLfloat) day, 0.0, 0.0);
   glTranslatef (1.0, 0.0, 0.0);
   printPlanet(4, rdMoon, 1.0, 1.0, 1.0);
   glPopMatrix();


   // MARS
   glPushMatrix();
   glRotatef (year/1.88, 0, 1, 0); // 686 days
   glTranslatef (7, 0.0, 0.0);
   glRotatef (day, 0.0, 1.0, 0.0);
   glRotatef (25.19, 1.0, 0.0, 0.0);
   printPlanet(5, rdSmall, 1.0, 1.0, 1.0);
   glPopMatrix();
   
   
   // JUPITER
   glPushMatrix();
   glRotatef (year/11.86, 0, 1, 0); // 11 years and 314 days
   glTranslatef (8, 0.0, 0.0);
   glRotatef (day/2.4, 0.0, 1.0, 0.0);
   glRotatef (3.13, 1.0, 0.0, 0.0);
   printPlanet(6, rdBig, 1.0, 1.0, 1.0);
   glPopMatrix();
   
   
   // SATURN
   glPushMatrix();
   glRotatef (year/29.46, 0, 1, 0); // 29 years and 167 days
   glTranslatef (10, 0.0, 0.0);
   glRotatef (day/2.29, 0.0, 1.0, 0.0);
   glRotatef (26.73, 1.0, 0.0, 0.0);
   printPlanet(7, rdBig, 1.0, 1.0, 1.0);
   drawSaturnRing();
   glPopMatrix();
   
   
   // URANUS
   glPushMatrix();
   glRotatef (year/84.02, 0, 1, 0); // 84 years and 7 days
   glTranslatef (12, 0.0, 0.0);
   glRotatef (day/1.41, 0.0, 1.0, 0.0);
   glRotatef (97.77, 1.0, 0.0, 0.0);
   printPlanet(8, rdSmall, 1.0, 1.0, 1.0);
   glPopMatrix();
   
   // NEPTUNE
   glPushMatrix();
   glRotatef (year/164.77, 0, 1, 0); // 164 years and 280 days
   glTranslatef (13, 0.0, 0.0);
   glRotatef (day/1.5, 0.0, 1.0, 0.0);
   glRotatef (28.32, 1.0, 0.0, 0.0);
   printPlanet(9, rdSmall, 1.0, 1.0, 1.0);
   glPopMatrix();

   
   glFlush();
   glutSwapBuffers();
}

void printPlanet(int textureIdx, float radix, float r, float g, float b){
   glEnable(GL_TEXTURE_2D);
   GLUquadricObj *sphere = gluNewQuadric();
   gluQuadricDrawStyle(sphere, GLU_FILL);
   gluQuadricTexture(sphere, GL_TRUE);
   gluQuadricNormals(sphere, GLU_SMOOTH);
   glBindTexture(GL_TEXTURE_2D, textures[textureIdx]);
   glColor3f(r, g, b);
   gluSphere(sphere, radix, slices, stacks);
   gluDeleteQuadric(sphere);
   glDisable(GL_TEXTURE_2D);
}

void myinit (void) {
    glShadeModel (GL_FLAT);
    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    glClearDepth( 1.0 );
    glEnable(GL_COLOR_MATERIAL);
    glDisable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0,0,0,1);
    glCullFace(GL_BACK);

    glEnable(GL_LIGHTING);

    // Light values and coordinates
    glEnable(GL_DEPTH_TEST);	// Hidden surface removal
    glFrontFace(GL_CCW);		// Counter clock-wise polygons face out
    glEnable(GL_CULL_FACE);		// Do not calculate inside of jet
    
    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);   // Enable color tracking

	// Setup and enable light 0
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,whiteLight);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,sourceLight);
	//glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
	glEnable(GL_LIGHT0);

	// Set Material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

   if (!texturesLoaded) {
       textures[0] = LoadTexture("sun.rgb");
       textures[1] = LoadTexture("mercury.rgb");
       textures[2] = LoadTexture("venus.rgb");
       textures[3] = LoadTexture("earth.rgb");
       textures[4] = LoadTexture("moon.rgb");
       textures[5] = LoadTexture("mars.rgb");
       textures[6] = LoadTexture("jupiter.rgb");
       textures[7] = LoadTexture("saturn.rgb");
       textures[8] = LoadTexture("uranus.rgb");
       textures[9] = LoadTexture("neptune.rgb");
       textures[10] = LoadTexture("saturnringmap.rgb");
       textures[11] = LoadTexture("stars.rgb");
       texturesLoaded = true;
   }

}

void  myReshape(int w, int h) {
   h = (h == 0) ? 1 : h;
   w = (w == 0) ? 1 : w;
   glViewport(0, 0,(GLsizei) w,(GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(90*zoom_factor, w/h, 1.0, 500);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   current_w = w;
   current_h = h;
}

static void idle(void){
      if(animate) {
        day += (moonMovementByHour*velocity);
        year += (planetMovementByHour*velocity);
        //Sleep(50);
        glutPostRedisplay();
     }
}



void mouse_movement (int x, int y) {

	if ((current_w/2) > x)
       sunX -= 0.1;
	else
       sunX += 0.1;

	if ((current_h/2) > y)
       sunY -= 0.1;
	else
       sunY += 0.1;

	glutPostRedisplay();
}






void coords(){
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

void drawSaturnRing() {
	GLUquadricObj* quadro = gluNewQuadric();							
	gluQuadricNormals(quadro, GLU_SMOOTH);		
	gluQuadricTexture(quadro, GL_TRUE);			
	glEnable(GL_TEXTURE_2D);
		glPushMatrix();
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glPushMatrix();
				glRotatef(10, 0.0, 0.0, 1.0);
				glRotatef( 0, 0.0, 1.0, 0.0);
				glTranslatef(0, 0.0, 0.0 );
				glRotatef( -90.0, 1.0, 0.0, 0.0 );
				glBindTexture(GL_TEXTURE_2D, textures[10]);
				glScalef(1,1,.02);
				gluSphere(quadro, 1, 48, 48);
			glPopMatrix();
		glPopMatrix();
	gluDeleteQuadric(quadro);	
}

GLuint LoadTexture( const char * filename ) {
    GLuint texture;
    int width, height;
    unsigned char * data;
    FILE * file;
    file = fopen( filename, "rb" );
    if ( file == NULL ) return 0;
    width = 1024;
    height = 512;
    data = (unsigned char *)malloc( width * height * 3 );
    //int size = fseek(file,);
    fread( data, width * height * 3, 1, file );
    fclose( file );
    
    for(int i = 0; i < width * height ; ++i) {
        int index = i*3;
        unsigned char B,R;
        B = data[index];
        R = data[index+2];
        
        data[index] = R;
        data[index+2] = B;
    }

    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );
    
    
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data );
    free( data );
    
    return texture;
}

