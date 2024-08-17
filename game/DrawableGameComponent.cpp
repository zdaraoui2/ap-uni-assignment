#include "DrawableGameComponent.h"
#include <iostream>
#include <cstdlib>

DrawableGameComponent::DrawableGameComponent(int x, int y)
    : x(0), y(0), direction(Right) {}

void DrawableGameComponent::Update(const tm *eventTime)
{
    std::cout << "DrawableGameComponent ID: " << getId() << ", Time: "
              << eventTime->tm_hour << ":"
              << eventTime->tm_min << ":"
              << eventTime->tm_sec << std::endl;

    if (direction == Up)
        y--;
    else if (direction == Down)
        y++;
    else if (direction == Left)
        x--;
    else if (direction == Right)
        x++;

    if (x < 0)
        x = 0;
    if (x > SCREEN_WIDTH)
        x = SCREEN_WIDTH;
    if (y < 0)
        y = 0;
    if (y > SCREEN_HEIGHT)
        y = SCREEN_HEIGHT;

    Draw();
    ChangeDirection();
}

void DrawableGameComponent::Draw()
{
    std::cout << "Direction: ";
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
    std::cout << ", Position: (" << x << ", " << y << ")" << std::endl;
}

void DrawableGameComponent::ChangeDirection()
{
    Direction newDirection;
    do
    {
        newDirection = static_cast<Direction>(rand() % 4);
    } while (newDirection == direction);

    direction = newDirection;
}
