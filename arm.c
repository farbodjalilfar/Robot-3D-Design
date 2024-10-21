#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <glut/glut.h>
#else
#include <windows.h>
#include <gl/glut.h>
#endif

void drawCylinder(float radius, float height) {
    GLUquadric *quad;
    quad = gluNewQuadric();
    gluCylinder(quad, radius, radius, height, 32, 32);
}

void drawArm() {
    // Apply a 90-degree rotation around the X-axis to position the arm vertically
    glPushMatrix();
    glRotatef(-100.0, 1.0, 0.0, 0.0);  // Rotate around X-axis by 90 degrees (to align with Y-axis)

    // Add a sphere for the shoulder joint
    glPushMatrix();
        glTranslatef(0.0f, 1.0f, 0.3f);  // Position the sphere at the top of the shoulder
        glutSolidSphere(0.5f, 35, 35);   // Create the shoulder sphere (radius 0.3)
    glPopMatrix();

    // Shoulder Cylinder
    glPushMatrix();
        glTranslatef(0.0f, 1.0f, -1.0f);  // Position the cylinder under the shoulder sphere
        drawCylinder(0.2f, 1.0f);         // Radius 0.2, height 1.0
    glPopMatrix();

    // Joint (shoulder to wrist)
    glPushMatrix();
        glTranslatef(0.0f, 1.0f, -1.0f);  // Move to the position between shoulder and wrist
        glutSolidSphere(0.23f, 35, 35);   // Create a sphere joint
    glPopMatrix();

    // Wrist Cylinder
    glPushMatrix();
        glTranslatef(0.0f, 1.0f, -2.0f);  // Position at wrist height
        drawCylinder(0.2f, 2.0f);         // Radius 0.2, height 1.0
    glPopMatrix();

    glPopMatrix();  // End the entire arm transformation
}
