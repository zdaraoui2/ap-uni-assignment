#include "Game.h"
#include "DrawableGameComponent.h"
#include <iostream>

// Standalone function to initialise the game
void Init()
{
    std::cout << "Initialising game" << std::endl;
}

// Standalone function to terminate the game
void Terminate()
{
    std::cout << "Terminating game" << std::endl;
}

int main()
{
    // Create a dynamic instance of Game
    Game *game = new Game(2);

    // Invoke SetInitialise passing it one of the standalone function addresses
    game->SetInitialise(Init);

    // Invoke SetTerminate passing it the other standalone function address.
    game->SetTerminate(Terminate);

    // Add a GameComponent object with an id of 1 to the game object
    GameComponent *component1 = new GameComponent();
    game->Add(component1);

    // Add a DrawableGameComponent object with an id of 2 and a position of x=0,y=0 to the game object
    DrawableGameComponent *component2 = new DrawableGameComponent(0, 0);
    game->Add(component2);

    // Invoke the game object’s run member function
    game->Run();

    delete game;
    delete component1;
    delete component2;

    return 0;
}
