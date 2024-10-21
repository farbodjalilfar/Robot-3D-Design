#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <glut/glut.h>
#else
#include <windows.h>
#include <gl/glut.h>
#endif
#include "torso.h"

// Function to draw a triangular prism
void drawTriangularPrism(float scaleX, float scaleY, float scaleZ) {
    glPushMatrix();
    
    // Move and scale the triangular prism to the desired location
    glTranslatef(0.0, -2.5, -0.5);  // Adjust these values to position the prism under the cube

    glScalef(scaleX, scaleY, scaleZ);

    // Begin drawing the triangular prism

    // Draw the front face (triangle facing the Z-axis, with the flat side on top)
    glBegin(GL_TRIANGLES);
        glVertex3f(-0.5, 1.0, 0.0); // Top left (X-axis)
        glVertex3f(0.5, 1.0, 0.0);  // Top right (X-axis)
        glVertex3f(0.0, 0.0, 0.0);  // Bottom middle (sharp point downwards)
    glEnd();

    // Draw the back face (triangle parallel to the front, but along the Z-axis)
    glBegin(GL_TRIANGLES);
        glVertex3f(-0.5, 1.0, -1.0); // Top left (X-axis)
        glVertex3f(0.5, 1.0, -1.0);  // Top right (X-axis)
        glVertex3f(0.0, 0.0, -1.0);  // Bottom middle (sharp point downwards)
    glEnd();

    // Draw the top rectangle (connecting the two triangle flat edges, flat side up)
    glBegin(GL_QUADS);
        glVertex3f(-0.5, 1.0, 0.0);  // Front top left
        glVertex3f(0.5, 1.0, 0.0);   // Front top right
        glVertex3f(0.5, 1.0, -1.0);  // Back top right
        glVertex3f(-0.5, 1.0, -1.0); // Back top left
    glEnd();

    // Draw the left rectangle (connecting the left edges of the triangles)
    glBegin(GL_QUADS);
        glVertex3f(-0.5, 1.0, 0.0);  // Front top left
        glVertex3f(-0.5, 1.0, -1.0); // Back top left
        glVertex3f(0.0, 0.0, -1.0);  // Back bottom middle (sharp point)
        glVertex3f(0.0, 0.0, 0.0);   // Front bottom middle (sharp point)
    glEnd();

    // Draw the right rectangle (connecting the right edges of the triangles)
    glBegin(GL_QUADS);
        glVertex3f(0.5, 1.0, 0.0);   // Front top right
        glVertex3f(0.5, 1.0, -1.0);  // Back top right
        glVertex3f(0.0, 0.0, -1.0);  // Back bottom middle (sharp point)
        glVertex3f(0.0, 0.0, 0.0);   // Front bottom middle (sharp point)
    glEnd();

    glPopMatrix();
}


// Draw the torso
void drawTorso() {
    GLUquadric *quad = gluNewQuadric(); // Create a new quadric object for the cylinder

    // Draw the triangular prism underneath the cube
    drawTriangularPrism(2, 1, 1);

    // Push matrix for the cube underneath the torso
    glPushMatrix();
    glColor3f(0.3f, 0.3f, 0.3f);
    // Move and scale the cube underneath the torso
    glTranslatef(0.0, -1.0, -1.0);  // Move the cube downward (adjust values as needed)
    glScalef(1.0, 1.0, 1.0);        // Scale the cube to desired proportions
    glutSolidCube(1.0);              // Cube representing the base
    glColor3f(0.13f, 0.13f, 0.13f);
    glPopMatrix(); // End transformation for the lower cube

    // Push matrix for the torso
    glPushMatrix();
    
    // Move the torso (adjust the values of x, y, z for desired position)
    glTranslatef(0.0, 0.2, -1.0);  // Move the torso downward slightly and backward
    
    // Scale and draw the torso
    glScalef(1.7, 1.5, 1.5);       // Scale the torso
    glutSolidCube(1.0);             // Cube representing the torso
    
    glPopMatrix(); // End torso transformation

    // Now add the cylinder on top of the torso
    glPushMatrix();

    // Move to the top of the torso to place the cylinder
    glTranslatef(-1.0, 0.95, -1.0);  // Adjust this to place the cylinder on top of the torso
    
    // Rotate the cylinder 90 degrees around the Y-axis
    glRotatef(90.0, 0.0, 1.0, 0.0);  // Rotate 90 degrees over Y-axis
    
    // Draw the cylinder
    gluCylinder(quad, 0.75, 0.75, 2, 32, 32);  // Cylinder with radius 0.75 and height 2
    
    // Add the bottom cap of the cylinder
    glPushMatrix();
    gluDisk(quad, 0.0, 0.75, 32, 1);  // Bottom cap
    glPopMatrix();
    
    // Add the top cap of the cylinder
    glPushMatrix();
    glTranslatef(0.0, 0.0, 2.0);  // Move to the top of the cylinder
    gluDisk(quad, 0.0, 0.75, 32, 1);  // Top cap
    glPopMatrix();
    
    glPopMatrix();  // End cylinder transformation
    
    gluDeleteQuadric(quad); // Free the quadric object
}
