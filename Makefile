# gsh: simple Linux Group SHell
#
# Alan Herculano Diniz & Rafael Belmock Pedruzzi
#
# Project's build file

CC = gcc # Project's compiler
INC = -Iinclude # Include path (no need to specify the path of header in #include)
EXE = gsh # Executable file name

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
all:
	$(CC) -o $(EXE) $(wildcard src/*.c) $(INC) $(CMP_FLAGS) $(C_FLAGS)
