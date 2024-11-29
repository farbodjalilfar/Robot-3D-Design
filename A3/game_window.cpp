#include <GLUT/glut.h>
#include <iostream>
#include <cmath>

// Window dimensions
const int WIDTH = 1280;
const int HEIGHT = 720;

// Camera parameters
float cameraX = 0.0f, cameraY = 10.0f, cameraZ = 20.0f;  // Camera position
float lookAtX = 0.0f, lookAtY = 0.0f, lookAtZ = 0.0f;    // Look-at point
float cameraAngleY = 0.0f;  // Yaw (horizontal rotation)
float cameraAngleX = 0.0f;  // Pitch (vertical rotation)
bool isMousePressed = false;
int lastMouseX, lastMouseY;

// Initialize OpenGL settings
void initGL() {
    glEnable(GL_DEPTH_TEST);  // Enable depth testing
    glClearColor(0.1f, 0.1f, 0.8f, 1.0f);  // Set background color to dark gray
}

// Update the camera based on its angles
void updateCamera() {
    // Calculate the look-at point using spherical coordinates
    lookAtX = cameraX + cos(cameraAngleY) * cos(cameraAngleX);
    lookAtY = cameraY + sin(cameraAngleX);
    lookAtZ = cameraZ + sin(cameraAngleY) * cos(cameraAngleX);
}

// Display callback function
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear color and depth buffers

    // Set up the camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ, lookAtX, lookAtY, lookAtZ, 0.0, 1.0, 0.0);

    // Draw ground
    glColor3f(0.5f, 0.5f, 0.5f);  // Set ground color (green)
    glBegin(GL_QUADS);
        glVertex3f(-100.0f, 0.0f, -100.0f);  // Bottom-left corner
        glVertex3f(-100.0f, 0.0f,  100.0f);  // Top-left corner
        glVertex3f( 100.0f, 0.0f,  100.0f);  // Top-right corner
        glVertex3f( 100.0f, 0.0f, -100.0f);  // Bottom-right corner
    glEnd();

    glutSwapBuffers();  // Swap front and back buffers
}

// Reshape callback function
void reshape(int w, int h) {
    glViewport(0, 0, w, h);  // Adjust viewport

    glMatrixMode(GL_PROJECTION);  // Switch to projection matrix
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 100.0);  // Perspective projection
}

// Mouse motion callback
void mouseMotion(int x, int y) {
    if (isMousePressed) {
        // Calculate the mouse delta
        int deltaX = x - lastMouseX;
        int deltaY = y - lastMouseY;

        // Update camera angles based on mouse movement
        cameraAngleY += deltaX * 0.005f;  // Adjust yaw
        cameraAngleX -= deltaY * 0.005f;  // Adjust pitch

        // Clamp the pitch to avoid flipping the camera
        if (cameraAngleX > 1.5f) cameraAngleX = 1.5f;
        if (cameraAngleX < -1.5f) cameraAngleX = -1.5f;

        // Update the look-at point
        updateCamera();

        // Save the current mouse position
        lastMouseX = x;
        lastMouseY = y;

        glutPostRedisplay();  // Request a redraw
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
    glutInit(&argc, argv);  // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);  // Double buffering, RGB, depth buffer
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Mouse-Controlled Camera");

    initGL();  // Initialize OpenGL

    // Register callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMotionFunc(mouseMotion);
    glutMouseFunc(mouseButton);

    // Set initial camera angles
    updateCamera();

    glutMainLoop();  // Enter the GLUT event-processing loop
    return 0;
}
