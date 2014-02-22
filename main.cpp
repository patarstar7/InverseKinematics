
// glut stuff
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <math.h>
#include <vector>
#include <cmath>
#include <stdlib.h>

#include "algebra3.h"

#include "Arm.h"
#include "CircleTrack.h"
#include "EightTrack.h"
#include "SpiralTrack.h"

using namespace std;

//****************************************************
// Some Classes
//****************************************************

class Viewport;

class Viewport {
  public:
    int w, h; // width and height
};


//****************************************************
// Global Variables
//****************************************************

Viewport	viewport;
double zoomfactor = 0.8;
GLfloat viewangle = 0, tippangle = 0;

int ikRate = 30720;

bool mouseDown = false;
float xrot = 0.0f;
float yrot = 0.0f;
float xdiff = 0.0f;
float ydiff = 0.0f;

int play = 1;
int tracksActive = 1;

vector<Arm*> arms;
vector<vec3*> targets;
vector<TargetTrack*> targetTracks;

string mode;

double pi = 3.14159265359;


//***************************************************************
// change target coordinates
//***************************************************************
void changeTarget(int which, vec3 change) {
  (*targets[which])[VX] = change[VX];
  (*targets[which])[VY] = change[VY];
  (*targets[which])[VZ] = change[VZ];
}

void moveTarget(int which, vec3 change) {
  (*targets[which])[VX] += change[VX];
  (*targets[which])[VY] += change[VY];
  (*targets[which])[VZ] += change[VZ];
}

//****************************************************
// mouse functions
//****************************************************
void mouse(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    mouseDown = true;
     
    xdiff = x - yrot;
    ydiff = -y + xrot;
  }
  else{
   mouseDown = false;
  }
}
 
void mouseMotion(int x, int y)
{
  if (mouseDown)
  {
  yrot = x - xdiff;
  xrot = y + ydiff;
   
  glutPostRedisplay();
  }
}

//***************************************************
// function that takes in keyboard commands
//***************************************************
void myKeyboard(unsigned char Key, int x, int y)
{
  switch(Key)
  {
    case '=':
    case '+':  
      zoomfactor -= 0.1;
      break;
    case '-':  
      zoomfactor += 0.1;
      break;

    case '[':
      if(ikRate > 15)
        ikRate /= 2;
      cout << "ikRate = " << ikRate << endl;
      break;
    case ']':
      if(ikRate < 1006632960)
        ikRate *= 2;
      cout << "ikRate = " << ikRate << endl;
      break;

    case 'w':
      (*targets[0])[VY] += 0.1;
      break;
    case 'a':
      (*targets[0])[VX] -= 0.1;
      break;
    case 's':
      (*targets[0])[VY] -= 0.1;
      break;
    case 'd':
      (*targets[0])[VX] += 0.1;
      break;
    case 'q':
      (*targets[0])[VZ] += 0.1;
      break;
    case 'e':
      (*targets[0])[VZ] -= 0.1;
      break;

    case 'p':
      play = !play;
      cout << "play = " << play << endl;
      break;

    case 'x':
      // circling = 0;
      // eighting = 0;
      // spiraling = 0;
      break;
    case 'c':
      // eighting = 0;
      // spiraling = 0;
      // circling = !circling;
      // cout << "circling = " << circling << endl;
      break;
    case 'v':
      // spiraling = 0;
      // circling = 0;
      // eighting = !eighting;
      // cout << "eighting = " << eighting << endl;
      break;
    case 'b':
      // circling = 0;
      // eighting = 0;
      // spiraling = !spiraling;
      // cout << "spiraling = " << spiraling << endl;
      break;

    case 't':
      tracksActive = !tracksActive;
      cout << "tracksActive = " << tracksActive << endl;
      break;

    case '1':
      // mode = "transpose";
      // cout << "transpose mode" << endl;
      break;
    case '2':
      // mode = "ccd";
      // cout << "ccd mode" << endl;
      break;
    case '3':
      // mode = "pseudo";
      // cout << "pseudo mode" << endl;
      break;

    case 'm':
      if(mode == "transpose")
        mode = "ccd";
      else
        mode = "transpose";
      cout << "ik mode: " << mode << endl;
  };

  glutPostRedisplay();
}

//***************************************************
// function that takes in keyboard commands
//***************************************************
void keyUp (unsigned char key, int x, int y) {  
  switch(key)
  {
    case '/':
      for(int i = 0; i < arms.size(); i++)
        arms[i]->ik(mode);
      glutPostRedisplay();   
      break;
    case '.':
      glutPostRedisplay();
      break;
  };
  // glutPostRedisplay();
}


//***************************************************
// function that takes in keyboard commands
//***************************************************
void specialKey (int key, int x, int y) {  
  switch(key)
  {
    case GLUT_KEY_LEFT : 
      (*targets[1])[VX] -= 0.1;
      break;
    case GLUT_KEY_RIGHT:
      (*targets[1])[VX] += 0.1;
      break;
    case GLUT_KEY_UP:
      if(glutGetModifiers() == GLUT_ACTIVE_SHIFT)
        (*targets[1])[VZ] += 0.1;
      else
        (*targets[1])[VY] += 0.1;
      break;
    case GLUT_KEY_DOWN :
      if(glutGetModifiers() == GLUT_ACTIVE_SHIFT)
        (*targets[1])[VZ] -= 0.1;
      else
        (*targets[1])[VY] -= 0.1;
      break;


  };
  // glutPostRedisplay();
}


//****************************************************
// called by glut when there are no messages to handle
//****************************************************
void myFrameMove() {
    //nothing here for now
  #ifdef _WIN32
    Sleep(10);  //give ~10ms back to OS (so as not to waste the CPU)
  #endif
    glutPostRedisplay(); // forces glut to call the display function (myDisplay())
}

//****************************************************
// reshape viewport if the window is resized
//****************************************************
void myReshape(int w, int h) {
  viewport.w = w;
  viewport.h = h;

  glViewport (0,0,viewport.w,viewport.h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
}

//**************************************************
// update the screen at 30 fps
//*******************************************
void frameTimer(int value) {
  glutPostRedisplay();
  glutTimerFunc(1000/30, frameTimer, 1);
}

//**************************************************
// do IK calculations at rate defined by ikRate
//*******************************************
void ikTimer(int value) {
  if(play) {
    // if(circling)
    //   targetCircle();
    // else if(eighting)
    //   target8();
    // else if(spiraling)
    //   targetSpiral();

    // if(tracksActive) {
    //   for(int i = 0; i < targetTracks.size(); i++) {
    //     if(i >= targets.size()) {
    //       cout << "ERROR: more target tracks than targets" << endl;
    //       break;
    //     }
    //     changeTarget(i, targetTracks[i]->next());
    //   }
    // }

    for(int i = 0; i < arms.size(); i++) {
      arms[i]->ik(mode);
    }
  }
  glutTimerFunc(1000/ikRate, ikTimer, 1);
}

//****************************************************
// function that does the actual drawing of stuff
//***************************************************
void myDisplay() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION); // indicate we are specifying camera transformations
  glLoadIdentity(); // make sure transformation is "zero'd"
  gluPerspective(50.0 * zoomfactor, (float)viewport.w/(float)viewport.h, 0.1f, 100.f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0f, 1.5f, 5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

  glEnable(GL_DEPTH_TEST);

  glPushMatrix();

  glRotatef(xrot, 1.0f, 0.0f, 0.0f);
  glRotatef(yrot, 0.0f, 1.0f, 0.0f);

  // make platform
  GLfloat platSize = 1.0;
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glColor3f(0.3f, 0.3f, 0.3f);
  glBegin(GL_TRIANGLES);
  glVertex3d(platSize, 0.0f, -platSize);
  glVertex3d(platSize, 0.0f, platSize);
  glVertex3d(-platSize, 0.0f, platSize);

  glVertex3d(-platSize, 0.0f, platSize);
  glVertex3d(-platSize, 0.0f, -platSize);
  glVertex3d(platSize, 0.0f, -platSize);
  glEnd();

  // make target dot
  glPointSize(17.0f);
  glColor3f(1.0f, 0.0f, 0.0f);
  glBegin(GL_POINTS);
  for(int i = 0; i < targets.size(); i++) {
    glVertex3d((*targets[i])[VX], (*targets[i])[VY], (*targets[i])[VZ]);
  }
  glEnd();

  // if(circling)
  //   drawCircle();
  // else if(eighting)
  //   draw8();
  // else if(spiraling)
  //   drawSpiral();

  if(tracksActive) {
    for(int i = 0; i < targetTracks.size(); i++) {
      if(i >= targets.size()) {
        // cout << "ERROR: more target tracks than targets" << endl;
        break;
      }
      targetTracks[i]->render();
      changeTarget(i, targetTracks[i]->next());
    }
  }


  for(int i = 0; i < arms.size(); i++) {
    arms[i]->render();
  }

  glPopMatrix();

  glFlush();
  glutSwapBuffers();
}

//****************************************************
// construct the scene according to preset scenes specified by the arguments
//****************************************************
void handleArguments(int argc, char *argv[]) {

  int configuration = 1;
  int trackPreset = 1;

  if(argc >= 2)
    configuration = atoi(argv[1]);
  if(argc >= 3)
    trackPreset = atoi(argv[2]);

  if(configuration == 1) {
    Root* root = new Root();
    BallSegment* bs1 = new BallSegment(0.7, vec3(0.0, 0.0, 0.0));
    BallSegment* bs2 = new BallSegment(0.7, vec3(0.0, 0.7, 0.0));
    BallSegment* bs3 = new BallSegment(0.7, vec3(0.0, 1.4, 0.0));
    root->addNextSegment(bs1);
    bs1->addNextSegment(bs2);
    bs2->addNextSegment(bs3);
    Arm* arm1 = new Arm(root);
    arms.push_back(arm1);
    targets.push_back(new vec3(bs3->endPosition()));
    bs3->setTarget(targets[0]);
  } else if(configuration == 2) {
    Root* root = new Root(vec3(-0.5, 0, 0));
    BallSegment* bs1 = new BallSegment(0.7, vec3(-0.5, 0.0, 0.0));
    BallSegment* bs2 = new BallSegment(0.7, vec3(-0.5, 0.7, 0.0));
    BallSegment* bs3 = new BallSegment(0.7, vec3(-0.5, 1.4, 0.0));
    root->addNextSegment(bs1);
    bs1->addNextSegment(bs2);
    bs2->addNextSegment(bs3);
    Arm* arm1 = new Arm(root);
    arms.push_back(arm1);
    targets.push_back(new vec3(bs3->endPosition()));
    bs3->setTarget(targets[0]);

    Root* rootb = new Root(vec3(0.5, 0, 0));
    BallSegment* bs1b = new BallSegment(0.8, vec3(0.5, 0.0, 0.0));
    BallSegment* bs2b = new BallSegment(0.2, vec3(0.5, 0.8, 0.0));
    BallSegment* bs3b = new BallSegment(0.2, vec3(0.5, 1.0, 0.0));
    BallSegment* bs4b = new BallSegment(0.8, vec3(0.5, 1.2, 0.0));
    rootb->addNextSegment(bs1b);
    bs1b->addNextSegment(bs2b);
    bs2b->addNextSegment(bs3b);
    bs3b->addNextSegment(bs4b);
    Arm* arm2 = new Arm(rootb);
    arms.push_back(arm2);
    targets.push_back(new vec3(bs4b->endPosition()));
    bs4b->setTarget(targets[1]);
  } else if(configuration == 3) {
    Root* root = new Root(vec3(0, 0, 0));
    BallSegment* bs1 = new BallSegment(0.7, vec3(0.0, 0.0, 0.0));
    BallSegment* bs2a = new BallSegment(0.7, vec3(0.0, 0.7, 0.0));
    BallSegment* bs3a = new BallSegment(0.7, vec3(0.0, 1.4, 0.0));
    BallSegment* bs2b = new BallSegment(0.7, vec3(0.0, 0.7, 0.0));
    BallSegment* bs3b = new BallSegment(0.7, vec3(0.0, 1.4, 0.0));
    root->addNextSegment(bs1);
    bs1->addNextSegment(bs2a);
    bs2a->addNextSegment(bs3a);
    bs1->addNextSegment(bs2b);
    bs2b->addNextSegment(bs3b);
    Arm* arm1 = new Arm(root);
    arms.push_back(arm1);
    targets.push_back(new vec3(bs3a->endPosition()));
    targets.push_back(new vec3(bs3b->endPosition()));
    bs3a->setTarget(targets[0]);
    bs3b->setTarget(targets[1]);
  } else if(configuration == 4) {
    Root* root = new Root(vec3(0, 0, 0));
    BallSegment* bs1 = new BallSegment(0.3, root->endPosition());
    root->addNextSegment(bs1);
    PrismSegment* ps1 = new PrismSegment(0.7, bs1->endPosition());
    bs1->addNextSegment(ps1);
    BallSegment* bs2a = new BallSegment(0.4, ps1->endPosition());
    ps1->addNextSegment(bs2a);
    BallSegment* ps2a = new BallSegment(0.5, bs2a->endPosition());
    bs2a->addNextSegment(ps2a);
    BallSegment* bs2b = new BallSegment(0.4, ps1->endPosition());
    ps1->addNextSegment(bs2b);
    PrismSegment* ps2b = new PrismSegment(0.7, bs2b->endPosition());
    bs2b->addNextSegment(ps2b);
    targets.push_back(new vec3(ps2b->endPosition()));
    ps2b->setTarget(targets[0]);
    targets.push_back(new vec3(ps2a->endPosition()));
    ps2a->setTarget(targets[1]);
    Arm* arm1 = new Arm(root);
    arms.push_back(arm1);
  } else if(configuration == 5) {
    Root* root = new Root(vec3(0, 0, 0));
    BallSegment* b1 = new BallSegment(0.7, root->endPosition());
    root->addNextSegment(b1);
    BallSegment* b2 = new BallSegment(0.7, b1->endPosition());
    b1->addNextSegment(b2);
    BallSegment* b3 = new BallSegment(0.7, b2->endPosition());
    b2->addNextSegment(b3);
    BallSegment* b4 = new BallSegment(0.7, b2->endPosition());
    b2->addNextSegment(b4);
    BallSegment* b8 = new BallSegment(0.7, b4->endPosition());
    b4->addNextSegment(b8);
    BallSegment* b5 = new BallSegment(0.7, b3->endPosition());
    b3->addNextSegment(b5);
    PrismSegment* p = new PrismSegment(1.0, b4->endPosition());
    b4->addNextSegment(p);
    BallSegment* b6 = new BallSegment(0.7, b8->endPosition());
    b8->addNextSegment(b6);
    BallSegment* b7 = new BallSegment(0.7, p->endPosition());
    p->addNextSegment(b7);
    targets.push_back(new vec3(b5->endPosition()));
    targets.push_back(new vec3(b6->endPosition()));
    targets.push_back(new vec3(b7->endPosition()));
    b5->setTarget(targets[0]);
    b6->setTarget(targets[1]);
    b7->setTarget(targets[2]);
    Arm* arm = new Arm(root);
    arms.push_back(arm);
  }

  if(trackPreset == 1) {
    CircleTrack* c = new CircleTrack();
    targetTracks.push_back(c);
    CircleTrack* c2 = new CircleTrack(vec3(-0.5, 0.9, 0.0), 0.6, 0);
    targetTracks.push_back(c2);
    CircleTrack* c3 = new CircleTrack(vec3(0.0, 1.5, 0.6), 1.4, 1);
    targetTracks.push_back(c3);
  } else if(trackPreset == 2) {
    EightTrack* e2 = new EightTrack(vec3(-0.9, 1.1, 0.0), 2);
    targetTracks.push_back(e2);
    CircleTrack* c = new CircleTrack();
    targetTracks.push_back(c);
    EightTrack* e = new EightTrack(vec3(0.0, 0.8, 0.5), 1);
    targetTracks.push_back(e);
  } else if(trackPreset == 3) {
    SpiralTrack* s = new SpiralTrack(0.8, 8);
    targetTracks.push_back(s);
    CircleTrack* c = new CircleTrack(vec3(1.2, 1.5, 0.0), 0.5, 2);
    targetTracks.push_back(c);
    CircleTrack* c2 = new CircleTrack(vec3(0.0, 1.5, 1.2), 0.5, 1);
    targetTracks.push_back(c2);
  }
}


//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {

  handleArguments(argc, argv);

  mode = "transpose";
  // mode = "ccd";

  //This initializes glut
  glutInit(&argc, argv);

  //This tells glut to use a double-buffered window with red, green, and blue channels 
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  // Initalize theviewport size
  viewport.w = 1200;
  viewport.h = 800;

  //The size and position of the window
  glutInitWindowSize(viewport.w, viewport.h);
  glutInitWindowPosition(0,0);
  glutCreateWindow(argv[0]);

  glutDisplayFunc(myDisplay);        // function to run when its time to draw something
  glutReshapeFunc(myReshape);        // function to run when the window gets resized
  glutKeyboardFunc(myKeyboard);
  glutKeyboardUpFunc(keyUp);
  glutSpecialFunc(specialKey);
  glutMouseFunc(mouse);
  glutMotionFunc(mouseMotion);

  frameTimer(0);
  ikTimer(0);

  glutMainLoop();              // infinite loop that will keep drawing and resizing

  return 0;
}
