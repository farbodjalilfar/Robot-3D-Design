#!/bin/bash

# Compile the C++ code
echo "Compiling a2MeshV..."
g++ meshViewer.cpp -o a2MeshV -framework OpenGL -framework GLUT -std=c++17

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. Running the program..."
    # Run the program
    ./a2MeshV "$@"
else
    echo "Compilation failed. Please check your code for errors."
    exit 1
fi
