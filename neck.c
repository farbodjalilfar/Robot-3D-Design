
#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <glut/glut.h>
#else
#include <windows.h>
#include <gl/glut.h>
#endif
#include "neck.h"

void drawNeck(float radius, float height) {
    GLUquadric *quad;
    quad = gluNewQuadric();
    
    glPushMatrix();
    
    // Rotate the neck along the X-axis by 90 degrees (to align vertically)
    glRotatef(90.0, 1.0, 0.0, 0.0);  // Rotate 90 degrees along the X-axis
    
    // Tilt the entire neck forward by 30 degrees
    glRotatef(30.0, 1.0, 0.0, 0.0);  // Tilt forward by 30 degrees

    // Draw the neck cylinder
    gluCylinder(quad, radius, radius, height, 32, 32);
    
    glPopMatrix();
    
    gluDeleteQuadric(quad); // Clean up
}
