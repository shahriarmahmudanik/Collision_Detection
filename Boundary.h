/*
 * Boundary.h
 * Boundary class that draws the walls
 */

#ifndef BOUNDARY_H
#define BOUNDARY_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class Boundary {
private:
    float left, right, bottom, top;  // Outer edges
    float thickness;                  // Wall thickness
    float r, g, b;                    // Wall color

public:
    // Constructor
    Boundary(float windowWidth, float windowHeight, float padding, float wallThickness)
        : thickness(wallThickness) {
        left = padding;
        right = windowWidth - padding;
        bottom = padding;
        top = windowHeight - padding;
        
        // Default color: dark grey
        r = 0.3f;
        g = 0.3f;
        b = 0.3f;
    }

    // Get inner boundaries (where ball bounces)
    float getInnerLeft() const { return left + thickness; }
    float getInnerRight() const { return right - thickness; }
    float getInnerBottom() const { return bottom + thickness; }
    float getInnerTop() const { return top - thickness; }

    // Draw the walls
    void draw() {
        glColor3f(r, g, b);
        
        // Bottom wall
        glBegin(GL_QUADS);
        glVertex2f(left, bottom);
        glVertex2f(right, bottom);
        glVertex2f(right, bottom + thickness);
        glVertex2f(left, bottom + thickness);
        glEnd();
        
        // Top wall
        glBegin(GL_QUADS);
        glVertex2f(left, top - thickness);
        glVertex2f(right, top - thickness);
        glVertex2f(right, top);
        glVertex2f(left, top);
        glEnd();
        
        // Left wall
        glBegin(GL_QUADS);
        glVertex2f(left, bottom);
        glVertex2f(left + thickness, bottom);
        glVertex2f(left + thickness, top);
        glVertex2f(left, top);
        glEnd();
        
        // Right wall
        glBegin(GL_QUADS);
        glVertex2f(right - thickness, bottom);
        glVertex2f(right, bottom);
        glVertex2f(right, top);
        glVertex2f(right - thickness, top);
        glEnd();
    }
};

#endif // BOUNDARY_H
