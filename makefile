# Makefile for lab10.cpp

# Compiler and flags
CC = g++
CFLAGS = -Wall -Wextra -std=c++11

# Targets (executables)
TARGETS = lab10

# Default target
all: $(TARGETS)

# Compile lab10
lab10: lab10.cpp
	$(CC) $(CFLAGS) -o lab10 lab10.cpp

# Clean up build files
clean:
	rm -f $(TARGETS)

# Run program
run: lab10
	./lab10