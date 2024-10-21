#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <glut/glut.h>
#else
#include <windows.h>
#include <gl/glut.h>
#endif
#include "cannon.h"

// Draw the leg (can be used for both left and right)
void drawCannon() {
    glPushMatrix();
    glScalef(0.4, 2.0, 0.4); // Scale the leg
    glutSolidCube(1.0);      // Cube representing the leg
    glPopMatrix();
}
