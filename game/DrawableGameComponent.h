#ifndef DRAWABLE_GAME_COMPONENT_H
#define DRAWABLE_GAME_COMPONENT_H

#include "GameComponent.h"

class DrawableGameComponent : public GameComponent
{
public:
    enum Direction
    {
        Left,
        Right,
        Up,
        Down
    };

    DrawableGameComponent(int x, int y);

    static const int SCREEN_HEIGHT = 100;
    static const int SCREEN_WIDTH = 100;

    void Update(const tm *eventTime) override;

private:
    void ChangeDirection();
    void Draw();

    Direction direction;
    int x, y;
};

#endif // DRAWABLE_GAME_COMPONENT_H
