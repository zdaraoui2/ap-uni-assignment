#include "DrawableGameComponent.h"
#include <iostream>
#include <cstdlib>

// Constructor that sets initial x, y to zero and direction to Right.
DrawableGameComponent::DrawableGameComponent(int x, int y)
    : x(0), y(0), direction(Right) {}

void DrawableGameComponent::Update(const tm *eventTime)
{
    // Display the component's ID and the current time.
    std::cout << "DrawableGameComponent ID: " << getId() << ", Time: "
              << eventTime->tm_hour << ":"
              << eventTime->tm_min << ":"
              << eventTime->tm_sec << std::endl;

    // Adjust position based on the current direction.
    if (direction == Up)
        y--;
    else if (direction == Down)
        y++;
    else if (direction == Left)
        x--;
    else if (direction == Right)
        x++;

    // Ensure the x and y positions don't go out of bounds (screen clamping).
    if (x < 0)
        x = 0;
    if (x > SCREEN_WIDTH)
        x = SCREEN_WIDTH;
    if (y < 0)
        y = 0;
    if (y > SCREEN_HEIGHT)
        y = SCREEN_HEIGHT;

    // Draw the updated position and direction.
    Draw();

    // Change to a new random direction after each update.
    ChangeDirection();
}

void DrawableGameComponent::Draw()
{
    std::cout << "Direction: ";

    // Display the current direction (Up, Down, Left, or Right).
    switch (direction)
    {
    case Up:
        std::cout << "Up";
        break;
    case Down:
        std::cout << "Down";
        break;
    case Left:
        std::cout << "Left";
        break;
    case Right:
        std::cout << "Right";
        break;
    }

    // Print the current x, y position.
    std::cout << ", Position: (" << x << ", " << y << ")" << std::endl;
}

void DrawableGameComponent::ChangeDirection()
{
    Direction newDirection;

    // Keep generating a random new direction until it's different from the current one.
    do
    {
        newDirection = static_cast<Direction>(rand() % 4);
    } while (newDirection == direction);

    direction = newDirection;
}
