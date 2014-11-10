#if LINUXVERSION==1
	#include "GL/glut.h"
#else
	#include "GLUT/glut.h"
#endif
#include "Scene.h"

Scene *sc = NULL;
const int timestep = 10;

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    sc->Render();

    glutSwapBuffers();
}

void timer(int value) { 
    // for all i do
    //     find neighborhoods Ni(t)
    // end for
    // for all i do
    //     compute density 
    //     compute pressure
    // end for
    // for all i do
    //     compute forces
    // end for
    // for all i do
    //     compute new velocity
    //     compute new position
    //     collision handling
    // end for

    sc->Update();

    glutTimerFunc(timestep, timer, 0);
    glutPostRedisplay(); 
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.1, 0.1, -float(h)/(10.0*float(w)), float(h)/(10.0*float(w)), 0.5, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

const GLfloat light_position[] = { 0.0f, 10.0f, -10.0f, 1.0f }; 
 
const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f }; 

void initScene() {   
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING); 

    glLightfv(GL_LIGHT0, GL_POSITION, light_position); 

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess); 

    sc = new Scene();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv); 

    // Sets up a double buffer with RGBA components and a depth component
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);

    // create window
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Hello!");

    // callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(timestep, timer, 0);

    // initialization
    initScene();

    // Starts the program.
    glutMainLoop();

    return 0;
}
