# Source files to compile
OBJS = source/main.c source/hashmap_generic.c

# Choose compiler
CC = gcc

# Compiler flags
COMPILER_FLAGS = -Wextra -Wall

# Build name and directory
OBJ_NAME = builds/driver

# Targets
compile : $(OBJS)
	cls
	$(CC) $(OBJS) $(COMPILER_FLAGS) -o $(OBJ_NAME)

run:
	builds/driver

compile_and_run: compile run
