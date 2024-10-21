#!/bin/bash
gcc input.c output.c errors.c scanner1.c -c
gcc input.o output.o errors.o scanner1.o main.c -o main
