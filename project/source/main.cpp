#include "GLUT/glut.h"
#include "scene.h"
#include "camera.h"

Scene *sc = NULL;
CCamera Camera;

bool play = false  ;
int fps = 30;

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    Camera.Render();

    sc->Render();
        
    glutSwapBuffers();
}

void render_timer(int value) { 

    glutPostRedisplay();
    glutTimerFunc(fps, render_timer, value);
}

void physical_timer(int value) { 
    if (play)
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
        // movement
        case 'a':       
            Camera.StrafeRight(-0.1);
            display();
            break;
        case 'd':       
            Camera.StrafeRight(0.1);
            display();
            break;
        case 'w':       
            Camera.MoveForwards(-0.1);
            display();
            break;
        case 's':       
            Camera.MoveForwards(0.1);
            display();
            break;
        case 'q':
            Camera.Move(F3dVector(0.0,-0.1,0.0));
            display();
            break;
        case 'e':
            Camera.Move(F3dVector(0.0,0.1,0.0));
            display();
            break;

        // rotation
        case 'j':       
            Camera.RotateY(1.0);
            display();
            break;
        case 'l':       
            Camera.RotateY(-1.0);
            display();
            break;
        case 'i':   
            Camera.RotateX(1.0);    
            display();
            break;
        case 'k':   
            Camera.RotateX(-1.0);    
            display();
            break;

        // rendering
        case '1':
            sc->render_boundary = !sc->render_boundary;
            cout << "boundary rendering: " << (sc->render_boundary ? "on" : "off") << endl;
            display();
            break;
        case '2':
            sc->boundary_force = !sc->boundary_force;
            cout << "boundary forces: " << (sc->boundary_force ? "on" : "off") << endl;
            display();
            break;
        case '3':
            sc->render_object = !sc->render_object;
            cout << "object rendering: " << (sc->render_object ? "on" : "off") << endl;
            display();
            break;
        case '4':
            sc->collide_object = !sc->collide_object;
            cout << "object: " << (sc->collide_object ? "on" : "off") << endl;
            display();
            break;

        // timestep
        case 'm':
            Scene::timestep = min(10, Scene::timestep+1);
            cout << "timestep: " << Scene::timestep << endl;
            break;    
        case 'n':
            Scene::timestep = max(1, Scene::timestep-1); 
            cout << "timestep: " << Scene::timestep << endl;
            break;

        // fps
        case 'b':
            fps = min(60, fps+1);
            cout << "fps: " << 1000/fps << endl;
            break;    
        case 'v':
            fps = max(1, fps-1); 
            cout << "fps: " << 1000/fps << endl;
            break;

        // pause / move step
        case 'p':
            play = !play;
            cout << (play ? "play" : "pause") << endl;
            display();
            break;
        case ' ':
            sc->Update();
            display();
            break;

        // reset
        case 'r':
            sc->Reset();
            display();
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
    glutTimerFunc(0, physical_timer, 0);
    glutTimerFunc(0, render_timer, 1);
    glutKeyboardFunc(keyboard);

    Camera.Move(F3dVector(0.0, 2.0, 1.0));
    Camera.RotateX(-10.0);
    Camera.MoveForwards(-3);

    // initialization
    initScene();

    // Starts the program.
    glutMainLoop();

    return 0;
}
