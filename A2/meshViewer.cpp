#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <glut/glut.h>
#else
#include <windows.h>
#include <gl/glut.h>
#endif

#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <cmath>

// Vertex structure for storing OBJ data
struct Vertex {
    float x, y, z;
};

struct Normal {
    float nx, ny, nz;
};

struct Face {
    int v1, v2, v3;  // Indices of vertices
    int n1, n2, n3;  // Indices of normals
};

// Mesh data
std::vector<Vertex> vertices;
std::vector<Normal> normals;
std::vector<Face> faces;

// Camera settings
float eyeX = 0.0f, eyeY = 3.0f, eyeZ = 10.0f;
float radius = 10.0f;
float theta = 0.0f, phi = 1.5f; // Spherical angles
float zoomFactor = 1.0f;
int lastMouseX, lastMouseY;
int currentButton;

// Flags for rendering modes
bool drawAsPoints = false;
bool drawAsLines = false;

// Function to load OBJ file
bool loadOBJ(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            Vertex v;
            iss >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        } else if (prefix == "vn") {
            Normal n;
            iss >> n.nx >> n.ny >> n.nz;
            normals.push_back(n);
        } else if (prefix == "f") {
            std::vector<int> vIndices, nIndices;
            std::string vertexInfo;
            while (iss >> vertexInfo) {
                std::istringstream viss(vertexInfo);
                int vIndex, nIndex;
                char dummy;
                viss >> vIndex >> dummy >> dummy >> nIndex;
                vIndices.push_back(vIndex - 1);
                nIndices.push_back(nIndex - 1);
            }

            if (vIndices.size() == 3) { // Triangle
                faces.push_back({vIndices[0], vIndices[1], vIndices[2],
                                nIndices[0], nIndices[1], nIndices[2]});
            } else if (vIndices.size() == 4) { // Quad (split into 2 triangles)
                faces.push_back({vIndices[0], vIndices[1], vIndices[2],
                                nIndices[0], nIndices[1], nIndices[2]});
                faces.push_back({vIndices[0], vIndices[2], vIndices[3],
                                nIndices[0], nIndices[2], nIndices[3]});
            }
        }
    }

    file.close();
    std::cout << "Loaded OBJ file: " << filename << "\n";
    return true;
}

// Initialize OpenGL settings
void init3DViewer() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);

    GLfloat lightPos[] = {10.0f, 10.0f, 10.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
}

// Display the mesh
void displayMesh() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Camera settings
    eyeX = radius * sin(phi) * cos(theta);
    eyeY = radius * cos(phi);
    eyeZ = radius * sin(phi) * sin(theta);

    gluLookAt(eyeX, eyeY, eyeZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    if (drawAsPoints) {
        glBegin(GL_POINTS);
    } else if (drawAsLines) {
        glBegin(GL_LINES);
    } else {
        glBegin(GL_TRIANGLES);
    }

    for (const auto& face : faces) {
        if (!normals.empty()) {
            glNormal3f(normals[face.n1].nx, normals[face.n1].ny, normals[face.n1].nz);
        }
        glVertex3f(vertices[face.v1].x, vertices[face.v1].y, vertices[face.v1].z);

        if (!normals.empty()) {
            glNormal3f(normals[face.n2].nx, normals[face.n2].ny, normals[face.n2].nz);
        }
        glVertex3f(vertices[face.v2].x, vertices[face.v2].y, vertices[face.v2].z);

        if (!normals.empty()) {
            glNormal3f(normals[face.n3].nx, normals[face.n3].ny, normals[face.n3].nz);
        }
        glVertex3f(vertices[face.v3].x, vertices[face.v3].y, vertices[face.v3].z);
    }
    glEnd();

    glutSwapBuffers();
}

// Mouse handling
void mouseButtonHandler(int button, int state, int x, int y) {
    currentButton = button;
    lastMouseX = x;
    lastMouseY = y;
}

// Mouse motion for rotation and zoom
void mouseMotionHandler(int x, int y) {
    int dx = x - lastMouseX;
    int dy = y - lastMouseY;

    if (currentButton == GLUT_LEFT_BUTTON) { // Rotate
        theta += dx * 0.01f;
        phi -= dy * 0.01f;
        if (phi < 0.1f) phi = 0.1f;
        if (phi > 3.1f) phi = 3.1f;
    } else if (currentButton == GLUT_RIGHT_BUTTON) { // Zoom
        radius += dy * 0.1f;
        if (radius < 1.0f) radius = 1.0f;
    }

    lastMouseX = x;
    lastMouseY = y;
    glutPostRedisplay();
}

// Keyboard handling
void keyboardHandler(unsigned char key, int x, int y) {
    switch (key) {
        case 'q':
        case 27: // Escape
            exit(0);
            break;
        case 'p':
            drawAsPoints = !drawAsPoints;
            drawAsLines = false;
            break;
        case 'l':
            drawAsLines = !drawAsLines;
            drawAsPoints = false;
            break;
        case 'r':
            drawAsPoints = drawAsLines = false;
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

// Resize window
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path to OBJ file>\n";
        return 1;
    }

    if (!loadOBJ(argv[1])) {
        return 1;
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Mesh Viewer");

    init3DViewer();
    glutDisplayFunc(displayMesh);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouseButtonHandler);
    glutMotionFunc(mouseMotionHandler);
    glutKeyboardFunc(keyboardHandler);

    glutMainLoop();
    return 0;
}
