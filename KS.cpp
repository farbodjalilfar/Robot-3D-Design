/*******************************************************************
	       Object Representations in OpenGL - Hierarchical Robot Model
********************************************************************/

#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <glut/glut.h>
#else
#include <windows.h>
#include <gl/glut.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// Adjustable parameters for robot parts
float head_scale_y = 0.67;  // Vertical scaling factor for the head
float zoom = 15.0;          // Distance from the robot (for zooming)
float rotate_x = 0.0;       // Rotation around x-axis (pitch)
float rotate_y = 0.0;       // Rotation around y-axis (yaw)
float arm_rotation_angle = 180.0f; // Global variable to store arm rotation angle
int rotate_cannon = 0;  // Global flag to toggle cannon rotation
float cannon_rotation_angle = 0.0f;  // Global variable to store cannon rotation angle
float leg_rotation_angle = 0.0f;  // Leg rotation angle
int rotate_leg = 0;          // Toggle for leg movement

// Include the header files for body parts
#include "head.h"
#include "neck.h"
#include "torso.h"
#include "arm.h"
#include "leg.h"

/* Initialize the system */
void init(int w, int h) {
    glClearColor(0.0, 0.4, 0.4, 0.0);
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0, 0.1, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Setup lighting and material properties
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_STENCIL_TEST);  // Enable the stencil buffer
    glClearStencil(0);          // Clear the stencil buffer

    // Set up the initial lighting properties
    GLfloat light_position[] = { 0.0, 10.0, 10.0, 1.0 };
    GLfloat light_ambient[]  = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat light_diffuse[]  = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glEnable(GL_COLOR_MATERIAL);  // Enable color material mode
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);  // Set color to control both ambient and diffuse
}

// Function to set material properties for metallic color (ensures proper lighting)
void setMaterialColor(float r, float g, float b) {
    GLfloat mat_ambient[] = { r * 0.3f, g * 0.3f, b * 0.3f, 1.0f };
    GLfloat mat_diffuse[] = { r, g, b, 1.0f };
    GLfloat mat_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };  // Slight metallic shine
    GLfloat mat_shininess[] = { 50.0f };
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

// Hierarchical robot design
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Apply zoom and rotation transformations
    gluLookAt(0.0, 2.0, zoom, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0);
    glRotatef(rotate_x, 1.0, 0.0, 0.0);  // Rotate around x-axis (pitch)
    glRotatef(rotate_y, 0.0, 1.0, 0.0);  // Rotate around y-axis (yaw)

    // Draw the robot using hierarchical modeling
    // Torso
    glPushMatrix();
    drawTorso();  // Call the torso drawing function from torso.c

    // Neck (relative to the torso)
    glPushMatrix();
    glTranslatef(0.0, 2.5, 0.0);  // Position the neck above the torso
    drawNeck(0.2, 1.2);  // Call the neck drawing function with radius 0.3 and height 1.2
    glPopMatrix();  // End neck transformation

    // Head (relative to the neck)
    glPushMatrix();
    glTranslatef(0.0, 2.5, 0.0);  // Position the head above the neck
    drawHead(1.0, head_scale_y);   // Call the head drawing function from head.c
    glPopMatrix(); // End head transformation

    // Left Arm (relative to the torso)
    glPushMatrix();
    glTranslatef(-1.2, 0, -1.3); // Position left arm relative to torso
    drawArm(0); // Call the arm drawing function from arm.c
    glPopMatrix(); // End left arm transformation

    // Right Arm (relative to the torso)
    glPushMatrix();
    glTranslatef(1.2, 0.0, -1.3); // Position right arm relative to torso
    drawArm(1); // Call the arm drawing function from arm.c
    glPopMatrix(); // End right arm transformation

    // Left Leg (relative to the torso)
    glPushMatrix();
    glTranslatef(-0.6, -2.7, 0.0); // Position left leg below torso
    drawLeg(1); // Call the leg drawing function from leg.c
    glPopMatrix(); // End left leg transformation

    // Right Leg (relative to the torso)
    glPushMatrix();
    glTranslatef(0.6, -2.7, 0.0); // Position right leg below torso
    drawLeg(-1); // Call the leg drawing function from leg.c
    glPopMatrix(); // End right leg transformation

    glPopMatrix(); // End torso transformation

    glutSwapBuffers();
}


/* Called when window is reshaped */
void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0, 0.1, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/* Handles input from arrow keys and special keys for zoom and rotation */
void arrowKeys(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
        rotate_y -= 5.0; // Rotate left (y-axis)
        glutPostRedisplay();
        break;
    case GLUT_KEY_RIGHT:
        rotate_y += 5.0; // Rotate right (y-axis)
        glutPostRedisplay();
        break;
    default:
        break;
    }
}

/* Handles input from standard keys for zoom and rotation */
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'w':
        zoom -= 0.5;  // Zoom in
        if (zoom < 5.0) zoom = 5.0;  // Limit minimum zoom
        glutPostRedisplay();
        break;
    case 's':
        zoom += 0.5;  // Zoom out
        if (zoom > 30.0) zoom = 30.0;  // Limit maximum zoom
        glutPostRedisplay();
        break;
    case 'a':
        rotate_x -= 5.0;  // Rotate up (x-axis)
        glutPostRedisplay();
        break;
    case 'd':
        rotate_x += 5.0;  // Rotate down (x-axis)
        glutPostRedisplay();
        break;
    case 'c': // Rotate arm counterclockwise
        arm_rotation_angle -= 5.0f;
        if (arm_rotation_angle < -360.0f) arm_rotation_angle += 360.0f;  // Reset after a full rotation
        glutPostRedisplay();
        break;
    case 'C': // Rotate arm clockwise
        arm_rotation_angle += 5.0f;
        if (arm_rotation_angle > 360.0f) arm_rotation_angle -= 360.0f;  // Reset after a full rotation
        break;
    case 'r':  // Toggle cannon rotation
        rotate_cannon = !rotate_cannon;  // Toggle the flag
        break;
    case 'l':  // Toggle leg rotation
        rotate_leg = !rotate_leg;  // Toggle the flag
        break;
    default:
        break;
    }
}

void update(int value) {
    // Update your scene here, for example by adjusting angles for rotation
    glutPostRedisplay();  // Request a redraw of the scene
    
    // Set up the next timer call in 1000ms / 30fps = ~33ms
    glutTimerFunc(33, update, 0);
}

/* Main Program */
int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init(500, 500);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(arrowKeys);    // For arrow keys and special keys
    glutKeyboardFunc(keyboard);    // For standard keys

    glutTimerFunc(33, update, 0);
    
    glutMainLoop();
    return 0;
}
