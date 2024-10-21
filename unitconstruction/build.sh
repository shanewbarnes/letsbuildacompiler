#!/bin/bash
gcc input.c output.c errors.c scanner.c parser.c codegen.c -c
gcc input.o output.o errors.o scanner.o parser.o codegen.o main.c -o main
