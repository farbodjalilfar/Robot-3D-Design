#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <glut/glut.h>
#else
#include <windows.h>
#include <gl/glut.h>
#endif
#include "torso.h"

// Draw the torso
void drawTorso() {
    glPushMatrix();
    
    // Move the torso (adjust the values of x, y, z for desired position)
    glTranslatef(0.0, 0.0, -0.5);  // Example: Move the torso upward by 1 unit in the Y-axis
    
    // Scale and draw the torso
    glScalef(1.0, 1.5, 0.7);     // Scale the torso
    glutSolidCube(2.0);           // Cube representing the torso
    
    glPopMatrix();
}
