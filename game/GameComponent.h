#ifndef GAME_COMPONENT_H
#define GAME_COMPONENT_H

#include <ctime>

class GameComponent
{
public:
    GameComponent();
    int getId() const;
    virtual void Update(const tm *eventTime);

private:
    int id;
    static int instances;
};

#endif // GAME_COMPONENT_H
