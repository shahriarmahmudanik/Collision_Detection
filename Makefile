# Makefile for 2D Collision Detection Project

# Compiler
CXX = g++

# Compiler flags
# GL_SILENCE_DEPRECATION silences OpenGL deprecation warnings on macOS
CXXFLAGS = -std=c++11 -Wall -Wextra -DGL_SILENCE_DEPRECATION

# Detect operating system and set appropriate flags
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    # macOS
    LDFLAGS = -framework OpenGL -framework GLUT
else
    # Linux
    LDFLAGS = -lGL -lGLU -lglut
endif

# Source files
SOURCES = main.cpp

# Header files
HEADERS = Vector2D.h Ball.h Boundary.h

# Output executable
TARGET = collision_sim

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)

# Clean build files
clean:
	rm -f $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Help
help:
	@echo "Available targets:"
	@echo "  make        - Build the project"
	@echo "  make clean  - Remove executable"
	@echo "  make run    - Build and run the project"
	@echo "  make help   - Show this help message"

.PHONY: all clean run help
