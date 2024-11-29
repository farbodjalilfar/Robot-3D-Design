#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <iostream>
#include <cmath>

// Include robot component headers
#include "robot/head.h"
#include "robot/neck.h"
#include "robot/torso.h"
#include "robot/arm.h"
#include "robot/leg.h"

// Window dimensions
const int WIDTH = 1280;
const int HEIGHT = 720;

// Camera parameters
float cameraX = 0.0f, cameraY = 4.0f, cameraZ = 20.0f;  // Camera position
float lookAtX = 0.0f, lookAtY = 0.0f, lookAtZ = 0.0f;    // Look-at point
float cameraAngleY = 0.0f;  // Yaw (horizontal rotation)
float cameraAngleX = 0.0f;  // Pitch (vertical rotation)
bool isMousePressed = false;
int lastMouseX, lastMouseY;

// Movement parameters
float moveSpeed = 0.5f;  // Speed of camera movement
float followSpeed = 0.02f;  // Speed at which robots follow the camera

// Robot structure
struct Robot {
    float x, y, z;    // Position
    float rotationY;  // Yaw angle to face the camera
};

// Robot positions
Robot robots[] = {
    {0.0f, 3.0f, 13.0f, 0.0f},
    {5.0f, 3.0f, -5.0f, 0.0f},
    {-5.0f, 3.0f, -5.0f, 0.0f},
    {0.0f, 3.0f, -10.0f, 0.0f}
};

// Initialize OpenGL settings
void initGL() {
    glEnable(GL_DEPTH_TEST);  // Enable depth testing
    glClearColor(0.1f, 0.1f, 0.8f, 1.0f);  // Set background color to dark blue
}

// Update the camera based on its angles
void updateCamera() {
    lookAtX = cameraX + cos(cameraAngleY) * cos(cameraAngleX);
    lookAtY = cameraY + sin(cameraAngleX);
    lookAtZ = cameraZ + sin(cameraAngleY) * cos(cameraAngleX);
}

// Move robots towards the camera and make them rotate to face it
void updateRobotPositions() {
    for (auto& robot : robots) {
        // Compute direction vector from robot to camera
        float dx = cameraX - robot.x;
        float dz = cameraZ - robot.z;

        // Calculate the angle to face the camera
        robot.rotationY = atan2(dz, dx) * 180.0f / M_PI;  // Convert radians to degrees

        // Normalize the direction vector
        float distance = sqrt(dx * dx + dz * dz);
        if (distance > 0.1f) {  // Prevent jittering when too close
            dx /= distance;
            dz /= distance;

            // Update robot position
            robot.x += dx * followSpeed;
            robot.z += dz * followSpeed;
        }
    }
}

// Draw a single robot at the specified position
void drawRobot(float x, float y, float z, float rotationY) {
    glPushMatrix();
    glTranslatef(x, y, z);         // Position the robot
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);  // Rotate to face the camera
    glScalef(0.5f, 0.5f, 0.5f);   // Scale down the robot to fit the scene

    // Torso
    glPushMatrix();
    drawTorso();

    // Neck
    glPushMatrix();
    glTranslatef(0.0f, 2.2f, 0.0f);  // Position above the torso
    drawNeck(0.2f, 1.2f);
    glPopMatrix();

    // Head
    glPushMatrix();
    glTranslatef(0.0f, 2.7f, 0.0f);  // Position above the neck
    drawHead(1.0f, 0.67f);
    glPopMatrix();

    // Arms
    glPushMatrix();
    glTranslatef(-1.2f, 0.0f, -1.5f);  // Left arm
    drawArm(0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.2f, 0.0f, -1.5f);  // Right arm
    drawArm(1);
    glPopMatrix();

    // Legs
    glPushMatrix();
    glTranslatef(-0.6f, -2.7f, 0.0f);  // Left leg
    drawLeg(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.6f, -2.7f, 0.0f);  // Right leg
    drawLeg(-1);
    glPopMatrix();

    glPopMatrix();  // End torso transformation
    glPopMatrix();  // End robot transformation
}

// Display callback function
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear buffers

    // Update robot positions
    updateRobotPositions();

    // Set up the camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ, lookAtX, lookAtY, lookAtZ, 0.0, 1.0, 0.0);

    // Draw the ground
    glColor3f(0.65f, 0.36f, 0.19f);
    glBegin(GL_QUADS);
        glVertex3f(-100.0f, 0.0f, -100.0f);
        glVertex3f(-100.0f, 0.0f,  100.0f);
        glVertex3f( 100.0f, 0.0f,  100.0f);
        glVertex3f( 100.0f, 0.0f, -100.0f);
    glEnd();

    // Draw all robots
    for (const auto& robot : robots) {
        drawRobot(robot.x, robot.y, robot.z, robot.rotationY);
    }

    glutSwapBuffers();  // Swap front and back buffers
}

// Reshape callback function
void reshape(int w, int h) {
    glViewport(0, 0, w, h);  // Adjust viewport
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 100.0);  // Perspective projection
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Handle WASD key presses for movement
void handleKeyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':  // Move forward
            cameraX += cos(cameraAngleY) * moveSpeed;
            cameraZ += sin(cameraAngleY) * moveSpeed;
            break;
        case 's':  // Move backward
            cameraX -= cos(cameraAngleY) * moveSpeed;
            cameraZ -= sin(cameraAngleY) * moveSpeed;
            break;
        case 'a':  // Strafe left
            cameraX -= sin(cameraAngleY) * moveSpeed;
            cameraZ += cos(cameraAngleY) * moveSpeed;
            break;
        case 'd':  // Strafe right
            cameraX += sin(cameraAngleY) * moveSpeed;
            cameraZ -= cos(cameraAngleY) * moveSpeed;
            break;
    }
    updateCamera();
    glutPostRedisplay();
}

// Mouse motion callback
void mouseMotion(int x, int y) {
    if (isMousePressed) {
        int deltaX = x - lastMouseX;
        int deltaY = y - lastMouseY;

        cameraAngleY += deltaX * 0.005f;
        cameraAngleX -= deltaY * 0.005f;

        if (cameraAngleX > 1.5f) cameraAngleX = 1.5f;
        if (cameraAngleX < -1.5f) cameraAngleX = -1.5f;

        updateCamera();

        lastMouseX = x;
        lastMouseY = y;

        glutPostRedisplay();
    }
}

// Mouse button callback
void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isMousePressed = true;
            lastMouseX = x;
            lastMouseY = y;
        } else if (state == GLUT_UP) {
            isMousePressed = false;
        }
    }
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Enemy Robots Following and Rotating Toward Camera");

    initGL();

    glutDisplayFunc(display);
    glutIdleFunc(display);  // Continuously update the scene
    glutReshapeFunc(reshape);
    glutKeyboardFunc(handleKeyboard);  // Register keyboard handler
    glutMotionFunc(mouseMotion);
    glutMouseFunc(mouseButton);

    updateCamera();

    glutMainLoop();
    return 0;
}
