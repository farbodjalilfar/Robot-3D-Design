#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <glut/glut.h>
#else
#include <windows.h>
#include <gl/glut.h>
#endif
extern float arm_rotation_angle;
extern float rotate_cannon;
extern float cannon_rotation_angle;

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
    glRotatef(arm_rotation_angle, 1.0, 0.0, 0.0);  // Rotate around the X-axis
    
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
        drawCylinder(0.2f, 2.0f);         // Radius 0.2, height 2.0
    glPopMatrix();

    if(hasCannon) {
        // Apply cannon rotation if the rotation is active
        if (rotate_cannon) {
            cannon_rotation_angle += 2.0f;  // Increment rotation
            if (cannon_rotation_angle > 360.0f) cannon_rotation_angle -= 360.0f;  // Keep within 360 degrees
        }

        glPushMatrix();
        // Rotate the cannon around the Z-axis
        glRotatef(cannon_rotation_angle, 0.0f, 0.0f, 1.0f);  // Rotate cannon if enabled

        // Draw the cube (cannon base)
        glPushMatrix();
            glTranslatef(0.0f, 0.0f, -2.6f);  // Position the cannon at the end of the arm
            glScalef(0.8f, 0.8f, 0.1f);      // Scale to the desired cannon size
            glutSolidCube(1.0);              // Draw the cube
        glPopMatrix();

        // Add a cylinder on top of the cube
        glPushMatrix();
            glTranslatef(0.2f, 0.2f, -3.0f);  // Position the cylinder slightly above the cube
            glRotatef(90, 0.0f, 0.0f, 1.0f);  // Rotate 90 degrees around the Y-axis
            drawCylinder(0.1f, 0.5f);         // Radius 0.2, height 1.0 (adjust as needed)
        glPopMatrix();

        glPushMatrix();
            glTranslatef(-0.2f, -0.2f, -3.0f);  // Position the cylinder slightly above the cube
            glRotatef(90, 0.0f, 0.0f, 1.0f);  // Rotate 90 degrees around the Y-axis
            drawCylinder(0.1f, 0.5f);         // Radius 0.2, height 1.0 (adjust as needed)
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.2f, -0.2f, -3.0f);  // Position the cylinder slightly above the cube
            glRotatef(90, 0.0f, 0.0f, 1.0f);  // Rotate 90 degrees around the Y-axis
            drawCylinder(0.1f, 0.5f);         // Radius 0.2, height 1.0 (adjust as needed)
        glPopMatrix();

        glPushMatrix();
            glTranslatef(-0.2f, 0.2f, -3.0f);  // Position the cylinder slightly above the cube
            glRotatef(90, 0.0f, 0.0f, 1.0f);  // Rotate 90 degrees around the Y-axis
            drawCylinder(0.1f, 0.5f);         // Radius 0.2, height 1.0 (adjust as needed)
        glPopMatrix();

        glPopMatrix();  // End rotation of cannon
    }

    glPopMatrix();  // End the entire arm transformation
}
