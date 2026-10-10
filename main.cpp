#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "Vector2D.h"
#include "Ball.h"
#include "Boundary.h"
#include "Obstacle.h"

// Window size
const int WIDTH = 800;
const int HEIGHT = 700;
const int NUM_BALLS = 25;

// Create objects
std::vector<Ball *> balls;
std::vector<Obstacle *> obstacles;
Boundary *boundary;

/*
 * Update - called every frame
 */
void update(int value)
{
    (void)value;

    // Move all balls
    for (size_t i = 0; i < balls.size(); i++)
    {
        balls[i]->move();
    }

    // Check wall collisions for all balls
    for (size_t i = 0; i < balls.size(); i++)
    {
        balls[i]->checkWallCollision(
            boundary->getInnerLeft(),
            boundary->getInnerRight(),
            boundary->getInnerBottom(),
            boundary->getInnerTop());
    }

    // Check ball-to-ball collisions (all pairs)
    for (size_t i = 0; i < balls.size(); i++)
    {
        for (size_t j = i + 1; j < balls.size(); j++)
        {
            balls[i]->checkBallCollision(*balls[j]);
        }
    }

    // Check ball-to-obstacle collisions
    for (size_t i = 0; i < balls.size(); i++)
    {
        for (size_t j = 0; j < obstacles.size(); j++)
        {
            if (obstacles[j]->checkCollision(
                    balls[i]->getPositionRef(),
                    balls[i]->getVelocityRef(),
                    balls[i]->getRadius()))
            {
                std::cout << "Ball " << i << " hit obstacle " << j << "!" << std::endl;
            }
        }
    }

    // Redraw
    glutPostRedisplay();

    // Call again after 16ms (~60 FPS)
    glutTimerFunc(16, update, 0);
}

/*
 * Display - draws everything
 */
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw boundary walls
    boundary->draw();

    // Draw all obstacles
    for (size_t i = 0; i < obstacles.size(); i++)
    {
        obstacles[i]->draw();
    }

    // Draw all balls
    for (size_t i = 0; i < balls.size(); i++)
    {
        balls[i]->draw();
    }

    glutSwapBuffers();
}

/*
 * Reshape - handle window resize
 */
void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
}

/*
 * Keyboard - handle key presses
 */
void keyboard(unsigned char key, int x, int y)
{
    (void)x;
    (void)y;
    if (key == 27 || key == 'q' || key == 'Q')
    {
        // Clean up balls
        for (size_t i = 0; i < balls.size(); i++)
        {
            delete balls[i];
        }
        balls.clear();

        // Clean up obstacles
        for (size_t i = 0; i < obstacles.size(); i++)
        {
            delete obstacles[i];
        }
        obstacles.clear();

        delete boundary;
        exit(0);
    }
}

/*
 * Main - program entry point
 */
int main(int argc, char **argv)
{
    // Seed random number generator
    srand(static_cast<unsigned>(time(nullptr)));

    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Collision Detection - Multiple Balls & Obstacles");

    // White background
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Create boundary (padding=40, wall thickness=15)
    boundary = new Boundary(WIDTH, HEIGHT, 10.0f, 5.0f);

    // Get inner boundaries for ball placement
    float innerLeft = boundary->getInnerLeft() + 30;
    float innerRight = boundary->getInnerRight() - 30;
    float innerBottom = boundary->getInnerBottom() + 30;
    float innerTop = boundary->getInnerTop() - 30;

    // Predefined colors for balls
    float colors[][3] = {
        {1.0f, 0.0f, 0.0f}, // Red
        {0.0f, 0.0f, 1.0f}, // Blue
        {0.0f, 0.8f, 0.0f}, // Green
        {1.0f, 0.5f, 0.0f}, // Orange
        {0.5f, 0.0f, 0.5f}, // Purple
        {0.0f, 0.8f, 0.8f}, // Cyan
        {1.0f, 0.0f, 1.0f}, // Magenta
        {0.6f, 0.4f, 0.2f}, // Brown
        {0.0f, 0.5f, 0.0f}, // Dark Green
        {0.2f, 0.2f, 0.8f}  // Dark Blue
    };
    int numColors = sizeof(colors) / sizeof(colors[0]);

    // Create balls with random positions and velocities
    for (int i = 0; i < NUM_BALLS; i++)
    {
        float x = innerLeft + static_cast<float>(rand()) / RAND_MAX * (innerRight - innerLeft);
        float y = innerBottom + static_cast<float>(rand()) / RAND_MAX * (innerTop - innerBottom);

        // Random velocity between -6 and 6, with minimum speed of 3.5
        float vx = (static_cast<float>(rand()) / RAND_MAX * 4.0f - 3.0f);
        float vy = (static_cast<float>(rand()) / RAND_MAX * 4.0f - 3.0f);
        if (std::abs(vx) < 3.5f)
            vx = vx < 0 ? -3.5f : 3.5f;
        if (std::abs(vy) < 3.5f)
            vy = vy < 0 ? -3.5f : 3.5f;

        // BALL SIZE - adjust ball radius
        Ball *ball = new Ball(x, y, vx, vy, 10.0f);
        ball->setColor(colors[i % numColors][0], colors[i % numColors][1], colors[i % numColors][2]);
        balls.push_back(ball);
    }

    // Create obstacles: Square, Triangle, Pentagon
    // Square obstacle (left-center area)
    Square *square = new Square(200, 300, 60);
    square->setColor(0.8f, 0.4f, 0.1f); // Orange-brown
    obstacles.push_back(square);

    // Triangle obstacle (center area)
    Triangle *triangle = new Triangle(400, 300, 70);
    triangle->setColor(0.2f, 0.6f, 0.2f); // Green
    obstacles.push_back(triangle);

    // Pentagon obstacle (right-center area)
    Pentagon *pentagon = new Pentagon(600, 300, 40);
    pentagon->setColor(0.3f, 0.3f, 0.8f); // Blue
    obstacles.push_back(pentagon);

    // Register callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, update, 0);

    std::cout << "=== Collision Detection ===" << std::endl;
    std::cout << "Number of balls: " << NUM_BALLS << std::endl;
    std::cout << "Obstacles: Square, Triangle, Pentagon" << std::endl;
    std::cout << "Press Q or ESC to quit" << std::endl;
    std::cout << "=====================" << std::endl;

    glutMainLoop();

    return 0;
}
