# gsh: simple Linux Group SHell
#
# Alan Herculano Diniz, Rafael Belmock Pedruzzi & Israel Santos
#
# Project's build file

CC = gcc # Project's compiler
INC_GSH = -Iinclude/gsh # -Iinclude/controller # Include path (no need to specify the path of header in #include)
GSH = bin/gsh # Executable file name
CONT = bin/controller # Auxiliar program file name

# If the config flag is not set, set it to "debug":
ifeq ($(config),)

config = debug

endif

# Debug configuration setup:
ifeq ($(config), debug)

CMP_FLAGS = -g -Wall -Wextra # Compilation flags
C_FLAGS = -DGSH_DEBUG # Creating an in-program flag to tell it's in debug mode (same as #define GSH_DEBUG)

endif

# Release configuration setup:
ifeq ($(config), release)

CMP_FLAGS = -O3 -w # Compilation flags
C_FLAGS = -DGSH_RELEASE # Creating an in-program flag to tell it's in debug mode (same as #define GSH_RELEASE)

endif

# Compilation target
gsh: controller
	$(CC) -o $(GSH) $(wildcard src/gsh/*.c) $(INC_GSH) $(CMP_FLAGS) $(C_FLAGS)

controller:
	$(CC) -o $(CONT) $(wildcard src/controller/*.c) $(CMP_FLAGS) $(C_FLAGS)
