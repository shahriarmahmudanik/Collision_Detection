/*
 * Ball.h
 * Ball class that handles its own drawing and movement
 */

#ifndef BALL_H
#define BALL_H

#include "Vector2D.h"
#include <cmath>
#include <iostream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class Ball
{
private:
    Vector2D position; // Ball position
    Vector2D velocity; // Ball velocity
    float radius;      // Ball radius
    float r, g, b;     // Ball color (RGB)

public:
    // Constructor
    Ball(float x, float y, float vx, float vy, float rad)
        : position(x, y), velocity(vx, vy), radius(rad)
    {
        // Default color: red
        r = 1.0f;
        g = 0.0f;
        b = 0.0f;
    }

    // Set ball color
    void setColor(float red, float green, float blue)
    {
        r = red;
        g = green;
        b = blue;
    }

    // Get position
    Vector2D getPosition() const { return position; }
    
    // Get position reference (for obstacle collision)
    Vector2D& getPositionRef() { return position; }
    
    // Get velocity reference (for obstacle collision)
    Vector2D& getVelocityRef() { return velocity; }

    // Get radius
    float getRadius() const { return radius; }

    // Move the ball based on velocity
    void move()
    {
        position += velocity;
    }

    // Check and handle collision with another ball
    bool checkBallCollision(Ball &other)
    {
        Vector2D diff = other.position - position;
        float distance = diff.magnitude();
        float minDistance = radius + other.radius;

        if (distance < minDistance)
        {
            // Collision detected
            std::cout << "Ball collision detected!" << std::endl;

            // Normalize the collision vector
            diff.normalize();

            // Simple elastic collision: swap velocities along collision axis
            Vector2D velAlongCollision1 = diff * (velocity.dot(diff));
            Vector2D velAlongCollision2 = diff * (other.velocity.dot(diff));

            velocity = velocity - velAlongCollision1 + velAlongCollision2;
            other.velocity = other.velocity - velAlongCollision2 + velAlongCollision1;

            // Separate balls to avoid overlap
            float overlap = minDistance - distance;
            Vector2D separation = diff * (overlap / 2.0f + 0.1f);
            position -= separation;
            other.position += separation;

            return true;
        }
        return false;
    }

    // Check and handle wall collisions
    void checkWallCollision(float left, float right, float bottom, float top)
    {
        // Left wall
        if (position.x - radius < left)
        {
            position.x = left + radius;
            velocity.reverseX();
            std::cout << "Hit LEFT wall!" << std::endl;
        }

        // Right wall
        if (position.x + radius > right)
        {
            position.x = right - radius;
            velocity.reverseX();
            std::cout << "Hit RIGHT wall!" << std::endl;
        }

        // Bottom wall
        if (position.y - radius < bottom)
        {
            position.y = bottom + radius;
            velocity.reverseY();
            std::cout << "Hit BOTTOM wall!" << std::endl;
        }

        // Top wall
        if (position.y + radius > top)
        {
            position.y = top - radius;
            velocity.reverseY();
            std::cout << "Hit TOP wall!" << std::endl;
        }
    }

    // Draw the ball
    void draw()
    {
        glColor3f(r, g, b);
        glBegin(GL_TRIANGLE_FAN);

        // Center of ball
        glVertex2f(position.x, position.y);

        // Draw circle with 36 segments
        for (int i = 0; i <= 36; i++)
        {
            float angle = 2.0f * 3.14159f * i / 36;
            float x = position.x + radius * cos(angle);
            float y = position.y + radius * sin(angle);
            glVertex2f(x, y);
        }
        glEnd();
    }
};

#endif // BALL_H
