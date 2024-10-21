#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <glut/glut.h>
#else
#include <windows.h>
#include <gl/glut.h>
#endif
extern float arm_rotation_angle;

void drawCylinder(float radius, float height) {
    GLUquadric *quad;
    quad = gluNewQuadric();
    gluCylinder(quad, radius, radius, height, 32, 32);
}

void drawArm(int hasCannon) {
    // Push the matrix before any transformations
    glPushMatrix();
    
    // Translate to the shoulder joint position (the center of rotation)
    glTranslatef(0.0f, 1.0f, 0.3f);  // Move to the shoulder joint
    
    // Rotate the entire arm based on the arm_rotation_angle
    glRotatef(arm_rotation_angle, 1.0, 0.0, 0.0);  // Rotate around the Z-axis
    
    // Now draw the shoulder joint sphere (center of rotation)
    glutSolidSphere(0.5f, 35, 35);   // Create the shoulder sphere (radius 0.3)

    // Draw the rest of the arm relative to the shoulder joint

    // Shoulder Cylinder
    glPushMatrix();
        glTranslatef(0.0f, 0.0f, -1.3f);  // Position the cylinder relative to the shoulder sphere
        drawCylinder(0.2f, 1.0f);         // Radius 0.2, height 1.0
    glPopMatrix();

    // Joint arm
    glPushMatrix();
        glTranslatef(0.0f, 0.0f, -1.3f);  // Position the joint sphere
        glutSolidSphere(0.23f, 35, 35);   // Create a sphere joint
    glPopMatrix();

    // Wrist Cylinder
    glPushMatrix();
        glTranslatef(0.0f, 0.0f, -2.6f);  // Position at wrist height relative to the shoulder
        drawCylinder(0.4f, 2.0f);         // Radius 0.2, height 2.0
    glPopMatrix();

    if(hasCannon){
        // Cannon holder Cylinder
        glPushMatrix();
            // glTranslatef(0.0f, 0.0f, -2.6f);  // Position at wrist height relative to the shoulder
            drawCylinder(0.2f, 0.2f);         // Radius 0.2, height 2.0
        glPopMatrix();
    }

    glPopMatrix();  // End the entire arm transformation
}
