# Source files to compile
OBJS = source/tests.c source/hashmap_generic.c

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

run_tests:
	builds/driver

compile_and_run_tests: compile run_tests
