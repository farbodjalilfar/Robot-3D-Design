#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <glut/glut.h>
#else
#include <windows.h>
#include <gl/glut.h>
#endif
#include "leg.h"
extern int rotate_leg;
extern float leg_rotation_angleR;
extern float knee_rotation_angleL;
extern float leg_rotation_angleL;
extern float knee_rotation_angleR;
int moveDirection = 1;
int steps = 90;

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
void drawLeg(int direction) {
    if (rotate_leg) {
        // if(leg_rotation_angle >= 35.0f || leg_rotation_angle <= -35.0f){
        //     moveDirection *= -1;
        // }

        // // Update the leg rotation angle based on the current direction
        // leg_rotation_angle += 1.0f * moveDirection;  // Adjust the speed of rotation here
        if(steps == 180){
            steps *= 0;
            rotate_leg = 0;
        }
        if(steps < 30){
            if(direction == 1){
                leg_rotation_angleR -= 1.5f;  // Adjust the speed of rotation here
                knee_rotation_angleR += 1.0f;  // Adjust the speed of rotation here
            }
        }else if(steps < 60){
            if(direction == 1){
                knee_rotation_angleR -= 1.0f;  // Adjust the speed of rotation here
            }
        }else if(steps < 90){
            if(direction == 1){
                leg_rotation_angleR += 1.5f;  // Adjust the speed of rotation here
            }
        }else if(steps < 120){
            if(direction == -1){
                leg_rotation_angleL -= 1.5f;  // Adjust the speed of rotation here
                knee_rotation_angleL += 1.0f;  // Adjust the speed of rotation here
            }
        }else if(steps < 150){
            if(direction == -1){
                knee_rotation_angleL -= 1.0f;  // Adjust the speed of rotation here
            }
        }else if(steps < 180){
            if(direction == -1){
                leg_rotation_angleL += 1.5f;  // Adjust the speed of rotation here
            }
        }
        steps += 1;
    }

    glPushMatrix();
        // Rotate the entire leg based on the leg_rotation_angle
        glTranslatef(0.0f, 1.0f, -1.0f);  // Adjust the position if necessary
        if(direction == 1){
            glRotatef(leg_rotation_angleR, 1.0f, 0.0f, 0.0f);  // Apply rotation to the leg
        }else{
            glRotatef(leg_rotation_angleL, 1.0f, 0.0f, 0.0f);  // Apply rotation to the leg
        }
    
        // Upper leg (vertical cylinder)
        glPushMatrix();
            glRotatef(90, 1.0f, 0.0f, 0.0f);   // Rotate around the X-axis to make the cylinder vertical
            glTranslatef(0.0f, 0.0f, 0.0f);    // Move the upper leg upwards along Y-axis
            drawLegCylinder(0.25f, 1.8f);       // Draw upper leg: radius 0.25, height 1.8
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.0f, -1.7f, 0.0f); 
            if(direction == 1){
                glRotatef(knee_rotation_angleR, 1.0f, 0.0f, 0.0f);
            }else{
                glRotatef(knee_rotation_angleL, 1.0f, 0.0f, 0.0f);
            }
            glColor3f(0.3f, 0.3f, 0.3f);
            // Knee joint (torus)
            glPushMatrix();
                glRotatef(-180, 1.0f, 0.0f, 0.0f);  // Rotate around the X-axis to make the torus vertical
                glRotatef(90, 0.0f, 1.0f, 0.0f);    // Additional rotation around the Y-axis
                glTranslatef(0.0f, 0, 0.0f);    // Position the torus between the upper and lower leg
                drawTorus(0.18f, 0.15f);            // Draw torus with inner radius 0.18 and outer radius 0.15
            glPopMatrix();
            glColor3f(0.13f, 0.13f, 0.13f); 

            // Lower leg (vertical cylinder)
            glPushMatrix();
                glRotatef(-90, 1.0f, 0.0f, 0.0f);   // Rotate around the X-axis to make the cylinder vertical
                glTranslatef(0.0f, 0.0f, -1.4f);    // Move the lower leg downwards along Y-axis
                drawLegCylinder(0.25f, 1.5f);       // Draw lower leg: radius 0.25, height 1.5
            glPopMatrix();

            glColor3f(0.3f, 0.3f, 0.3f);
            // Joint between leg and foot (torus)
            glPushMatrix();
                glRotatef(-180, 1.0f, 0.0f, 0.0f);  // Rotate around the X-axis to make the torus vertical
                glRotatef(90, 0.0f, 1.0f, 0.0f);    // Additional rotation around the Y-axis
                glTranslatef(0.0f, 1.3f, 0.0f);    // Position the torus between the lower leg and foot
                drawTorus(0.18f, 0.13f);            // Draw torus with inner radius 0.18 and outer radius 0.15
            glPopMatrix();
            glColor3f(0.13f, 0.13f, 0.13f);

            // Foot (connected to the lower leg)
            glPushMatrix();
                glTranslatef(0.0f, -1.5f, 0.3f);   // Move the foot downwards from the lower leg
                drawFoot();                         // Draw the foot
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}
