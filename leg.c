#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <glut/glut.h>
#else
#include <windows.h>
#include <gl/glut.h>
#endif
#include "leg.h"

// Function to draw a cylinder for the leg
void drawLegCylinder(float radius, float height) {
    GLUquadric *quad;
    quad = gluNewQuadric();
    gluCylinder(quad, radius, radius, height, 32, 32);
}

// Function to draw a torus (used as a joint)
void drawTorus(float innerRadius, float outerRadius) {
    glutSolidTorus(innerRadius, outerRadius, 30, 30);
}

// Function to draw a simple foot (using a cube or scaled box)
void drawFoot() {
    glPushMatrix();
        glScalef(0.5f, 0.15f, 0.8f);  // Scale the foot to be flat and wide
        glutSolidCube(1.0);           // Use a cube as the base shape for the foot
    glPopMatrix();
}

// Draw the leg (can be used for both left and right)
void drawLeg() {
    // Upper leg (vertical cylinder)
    glPushMatrix();
        glRotatef(-90, 1.0f, 0.0f, 0.0f);  // Rotate around the X-axis to make the cylinder vertical
        glTranslatef(0.0f, 1.0f, -1.0f);  // Move the upper leg upwards along Y-axis
        drawLegCylinder(0.25f, 1.8f);     // Draw upper leg: radius 0.25, height 1.8
    glPopMatrix();

    // Knee joint (torus) with rotation around Y-axis
    glPushMatrix();
        glRotatef(-180, 1.0f, 0.0f, 0.0f);  // Rotate around the X-axis to make the torus vertical
        glRotatef(90, 0.0f, 1.0f, 0.0f);    // Additional rotation around the Y-axis (90 degrees)
        glTranslatef(-1.0f, 1.2f, 0.0f);    // Position the torus between the upper and lower leg
        drawTorus(0.18f, 0.15f);            // Draw torus with inner radius 0.18 and outer radius 0.15
    glPopMatrix();

    // Lower leg (vertical cylinder)
    glPushMatrix();
        glRotatef(-90, 1.0f, 0.0f, 0.0f);  // Rotate around the X-axis to make the cylinder vertical
        glTranslatef(0.0f, 1.0f, -2.8f);   // Move the lower leg downwards along Y-axis
        drawLegCylinder(0.25f, 1.5f);      // Draw lower leg: radius 0.25, height 1.5
    glPopMatrix();

    // Joint between leg and foot (torus)
    glPushMatrix();
        glRotatef(-180, 1.0f, 0.0f, 0.0f);  // Rotate around the X-axis to make the torus vertical
        glRotatef(90, 0.0f, 1.0f, 0.0f);    // Additional rotation around the Y-axis (90 degrees)
        glTranslatef(-1.0f, 2.8f, 0.0f);    // Position the torus between the lower leg and foot
        drawTorus(0.18f, 0.13f);            // Draw torus with inner radius 0.18 and outer radius 0.15
    glPopMatrix();

    // Foot (connected to the lower leg)
    glPushMatrix();
        glTranslatef(0.0f, -3.0f, -0.8f);   // Move the foot downwards from the lower leg
        drawFoot();                         // Draw the foot
    glPopMatrix();
}
