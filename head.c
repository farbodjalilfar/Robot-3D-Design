#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <glut/glut.h>
#else
#include <windows.h>
#include <gl/glut.h>
#endif
#include "head.h"
extern float head_rotation_angle;

// Dynamic Clipping function
void setClippingPlane(float a, float b, float c, float d) {
    GLdouble clipPlane[] = {a, b, c, d};
    glClipPlane(GL_CLIP_PLANE0, clipPlane);
    glEnable(GL_CLIP_PLANE0);
}

// Disable clipping
void disableClippingPlane() {
    glDisable(GL_CLIP_PLANE0);
}

// Draw the head with dynamic clipping and full cylinder continuation
void drawHead(float radius, float scale_y) {
    GLUquadric* quad = gluNewQuadric();   // Create a new quadric object for the cylinder
    

    // Move the entire head slightly upwards on the Y-axis by 0.333 units
    glPushMatrix();
    glTranslatef(0.0, -0.333, 0.0);  // Move the head up by 1/3 of the previous amount (0.167)

    // Apply a uniform scale to the entire head
    glScalef(0.6, 0.6, 0.6);  // Scale the entire head and components

    // Apply the tilt to the whole head and cylinder assembly
    glRotatef(15.0, 1.0, 0.0, 0.0);  // Rotate the entire structure forward by 15 degrees along the X-axis
    glRotatef(head_rotation_angle, 0.0f, 1.0f, 0.0f); 

    glColor3f(0.13f, 0.13f, 0.13f);

    // Draw the head (sphere)
    glPushMatrix();
    setClippingPlane(0.0, 1.0, 0.0, 0);  // Clipping plane for the head
    glScalef(1.0, scale_y, 1.0);         // Scale the sphere vertically
    glutSolidSphere(radius, 20, 20);     // Sphere representing the head
    disableClippingPlane();
    glPopMatrix();

    // Draw the full cylinder below the head, with rotation over the X axis
    glPushMatrix();
    glTranslatef(0.0, radius * scale_y - 0.6, 0.0);  // Position the cylinder at the bottom of the sphere
    glRotatef(90.0, 1.0, 0.0, 0.0);                  // Rotate the cylinder along the X-axis

    // Draw the cylinder
    gluCylinder(quad, radius, radius, 0.5, 20, 10);  // Full cylinder: (top radius, bottom radius, height)

    // Draw the top and bottom caps of the cylinder to make it full
    glPushMatrix();
    gluDisk(quad, 0.0, radius, 20, 1);  // Top cap
    glTranslatef(0.0, 0.0, 0.5);        // Move to the bottom of the cylinder
    gluDisk(quad, 0.0, radius, 20, 1);  // Bottom cap
    glPopMatrix();

    // Eyes: Create holes on both sides of the cylinder (parallel)
    glPushMatrix();
    glColor3f(1, 1, 1);
    
    // Draw the left eye
    glTranslatef(-radius * 0.5, radius * 0.2 + 0.5, 0.0);  // Position the left eye hole
    glutSolidSphere(0.17, 20, 20);  // Eye hole size

    // Draw the right eye
    glTranslatef(radius, 0.0, 0.0);  // Position the right eye hole
    glutSolidSphere(0.17, 20, 20);  // Eye hole size

    glPopMatrix();  // End eye transformation

    glColor3f(0.06, 0.06, 0.06);
    // Add the "mouth" component using a cube
    glPushMatrix();
    glTranslatef(0.0, 0.95, 0.3);  // Adjust position: move cube forward and down relative to the face
    glScalef(0.4, 0.2, 0.6);       // Scale the cube to make it a rectangular mouth shape
    glutSolidCube(1.0);             // Draw the cube for the mouth
    glPopMatrix();
    glColor3f(0.13f, 0.13f, 0.13f);

    glPopMatrix();  // End of cylinder transformation

    glPopMatrix();  // End the overall head and cylinder transformation

    gluDeleteQuadric(quad);  // Free the quadric object
}
