#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
// Needed on MsWindows
#include <windows.h>
#endif // Win32 platform
 
#include <GLUT/glut.h>

#include "float2.h"
#include "float3.h"
#include "Mesh.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <vector>
#include <map>
#include "scene.h"

std::vector<bool> keysPressed;
// screen resolution
const int screenWidth = 1200;
const int screenHeight = 600;
bool HELICAM = true;

//scene object
Scene scene;
// Displays the image. 
void onDisplay( ) {
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear screen

  	scene.draw();
    glutSwapBuffers(); // drawing finished
}

void onIdle() {
	double t = glutGet(GLUT_ELAPSED_TIME) * 0.001;
	static double lastTime = 0.0;
	double dt = t - lastTime;
	lastTime = t;

	scene.getCamera().move(dt, keysPressed);
	glutPostRedisplay();
	scene.control(keysPressed);
	scene.move(t, dt, HELICAM);
}
void onKeyboard(unsigned char key, int x, int y) {
    keysPressed.at(key) = true;
    if(key == 'h') {
      HELICAM = !(HELICAM);
    }
}

void onKeyboardUp(unsigned char key, int x, int y) {
    keysPressed.at(key) = false;
}
void onMouse(int button, int state, int x, int y) {
  if(button == GLUT_LEFT_BUTTON)
    if(state == GLUT_DOWN)
      scene.getCamera().startDrag(x, y);
    else
      scene.getCamera().endDrag();
}

void onMouseMotion(int x, int y) {
  scene.getCamera().drag(x, y);
}
void onReshape(int winWidth, int winHeight) {
  glViewport(0, 0, winWidth, winHeight);
  scene.getCamera().setAspectRatio(
                    (float)winWidth/winHeight);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);						// initialize GLUT
    glutInitWindowSize(screenWidth, screenHeight);				// startup window size 
    glutInitWindowPosition(100, 100);           // where to put window on screen
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);    // 8 bit R,G,B,A + double buffer + depth buffer
 
    glutCreateWindow("OpenGL GAME");				// application window is created and displayed
 	  scene.initialize();
    glViewport(0, 0, screenWidth, screenHeight);
    glutDisplayFunc(onDisplay);					// register callback
    glutIdleFunc(onIdle);						// register callback
    glutKeyboardFunc(onKeyboard);
    glutKeyboardUpFunc(onKeyboardUp);
    for(int i=0; i<256; i++)
        keysPressed.push_back(false);
    glutMouseFunc(onMouse);
  	glutMotionFunc(onMouseMotion);
  	glutReshapeFunc(onReshape);


	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

    glutMainLoop();								// launch event handling loop
    
    return 0;
}