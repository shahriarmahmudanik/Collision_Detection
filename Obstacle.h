/*
 * Obstacle.h
 * Obstacle classes for collision detection (Square, Triangle, Pentagon)
 */

#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "Vector2D.h"
#include <cmath>
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Base class for all obstacles
class Obstacle {
protected:
    Vector2D position;  // Center position
    float size;         // Size of the obstacle
    float r, g, b;      // Color
    std::vector<Vector2D> vertices;  // Vertices of the polygon

public:
    Obstacle(float x, float y, float s) : position(x, y), size(s) {
        r = 0.5f; g = 0.5f; b = 0.5f;  // Default grey
    }

    virtual ~Obstacle() {}

    void setColor(float red, float green, float blue) {
        r = red; g = green; b = blue;
    }

    Vector2D getPosition() const { return position; }
    float getSize() const { return size; }
    const std::vector<Vector2D>& getVertices() const { return vertices; }

    // Get the closest point on a line segment to a point
    Vector2D closestPointOnSegment(const Vector2D& point, const Vector2D& a, const Vector2D& b) const {
        Vector2D ab = b - a;
        Vector2D ap = point - a;
        
        float t = ap.dot(ab) / (ab.dot(ab) + 0.0001f);
        t = std::max(0.0f, std::min(1.0f, t));
        
        return Vector2D(a.x + t * ab.x, a.y + t * ab.y);
    }

    // Get the normal vector of an edge (pointing outward)
    Vector2D getEdgeNormal(const Vector2D& a, const Vector2D& b) const {
        Vector2D edge = b - a;
        // Perpendicular vector (rotate 90 degrees)
        Vector2D normal(-edge.y, edge.x);
        normal.normalize();
        
        // Make sure normal points outward (away from center)
        Vector2D midpoint((a.x + b.x) / 2, (a.y + b.y) / 2);
        Vector2D toCenter = position - midpoint;
        
        if (normal.dot(toCenter) > 0) {
            normal.x = -normal.x;
            normal.y = -normal.y;
        }
        
        return normal;
    }

    // Check collision with a ball and return reflection info
    bool checkCollision(Vector2D& ballPos, Vector2D& ballVel, float ballRadius) {
        float minDist = 999999.0f;
        Vector2D closestPoint;
        Vector2D collisionNormal;
        int closestEdge = -1;

        // Check each edge of the polygon
        for (size_t i = 0; i < vertices.size(); i++) {
            size_t next = (i + 1) % vertices.size();
            Vector2D worldA = vertices[i] + position;
            Vector2D worldB = vertices[next] + position;
            
            Vector2D closest = closestPointOnSegment(ballPos, worldA, worldB);
            Vector2D diff = ballPos - closest;
            float dist = diff.magnitude();
            
            if (dist < minDist) {
                minDist = dist;
                closestPoint = closest;
                closestEdge = i;
            }
        }

        // Check if collision occurred
        if (minDist < ballRadius && closestEdge >= 0) {
            // Get the collision normal
            size_t next = (closestEdge + 1) % vertices.size();
            Vector2D worldA = vertices[closestEdge] + position;
            Vector2D worldB = vertices[next] + position;
            collisionNormal = getEdgeNormal(worldA, worldB);

            // Reflect the velocity
            float dotProduct = ballVel.dot(collisionNormal);
            ballVel.x = ballVel.x - 2.0f * dotProduct * collisionNormal.x;
            ballVel.y = ballVel.y - 2.0f * dotProduct * collisionNormal.y;

            // Push ball out of collision
            float overlap = ballRadius - minDist;
            ballPos.x += collisionNormal.x * (overlap + 1.0f);
            ballPos.y += collisionNormal.y * (overlap + 1.0f);

            return true;
        }
        return false;
    }

    // Draw the obstacle
    virtual void draw() {
        glColor3f(r, g, b);
        
        // Draw filled polygon
        glBegin(GL_POLYGON);
        for (const auto& v : vertices) {
            glVertex2f(position.x + v.x, position.y + v.y);
        }
        glEnd();
        
        // Draw outline
        glColor3f(r * 0.5f, g * 0.5f, b * 0.5f);
        glLineWidth(2.0f);
        glBegin(GL_LINE_LOOP);
        for (const auto& v : vertices) {
            glVertex2f(position.x + v.x, position.y + v.y);
        }
        glEnd();
    }
};

// Square obstacle
class Square : public Obstacle {
public:
    Square(float x, float y, float size) : Obstacle(x, y, size) {
        float half = size / 2.0f;
        vertices.push_back(Vector2D(-half, -half));
        vertices.push_back(Vector2D(half, -half));
        vertices.push_back(Vector2D(half, half));
        vertices.push_back(Vector2D(-half, half));
    }
};

// Triangle obstacle
class Triangle : public Obstacle {
public:
    Triangle(float x, float y, float size) : Obstacle(x, y, size) {
        float height = size * std::sqrt(3.0f) / 2.0f;
        vertices.push_back(Vector2D(0, height * 2.0f / 3.0f));           // Top
        vertices.push_back(Vector2D(-size / 2.0f, -height / 3.0f));      // Bottom left
        vertices.push_back(Vector2D(size / 2.0f, -height / 3.0f));       // Bottom right
    }
};

// Pentagon obstacle
class Pentagon : public Obstacle {
public:
    Pentagon(float x, float y, float size) : Obstacle(x, y, size) {
        for (int i = 0; i < 5; i++) {
            float angle = 2.0f * 3.14159f * i / 5.0f - 3.14159f / 2.0f;
            vertices.push_back(Vector2D(size * std::cos(angle), size * std::sin(angle)));
        }
    }
};

#endif // OBSTACLE_H
