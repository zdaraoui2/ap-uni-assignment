#ifndef GAME_H
#define GAME_H

#include "GameComponent.h"

typedef void (*FP)(void);

class Game
{
public:
    Game(int maxComponents);
    ~Game();
    void Add(GameComponent *component);
    void Run();
    void SetInitialise(FP init);
    void SetTerminate(FP term);

private:
    int componentCount;
    GameComponent **components;
    FP initialise;
    FP terminate;
    static const int TICKS_1000MS;
};

#endif // GAME_H
