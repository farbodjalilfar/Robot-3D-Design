#!/bin/bash
gcc -c robot/head.c robot/neck.c robot/torso.c robot/arm.c robot/leg.c  # Compile C files to object files
clang++ main.cpp head.o neck.o torso.o arm.o leg.o -o enemy_robots -framework OpenGL -framework GLUT
./enemy_robots