#include "GLUT/glut.h"
#include "scene.h"

Scene *sc = NULL;

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    sc->Render();

    glutSwapBuffers();
}

void render_timer(int value) { 

    glutPostRedisplay();
    glutTimerFunc(20, render_timer, value);
}

void physical_timer(int value) { 

    sc->Update();

    glutTimerFunc(Scene::timestep, physical_timer, value);
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
    // glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    // glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess); 

    sc = new Scene();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 'b':
            break;
    } 
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
    glutTimerFunc(Scene::timestep, physical_timer, 0);
    glutTimerFunc(30, render_timer, 1);
    glutKeyboardFunc(keyboard);

    // initialization
    initScene();

    // Starts the program.
    glutMainLoop();

    return 0;
}
