#include "Game.h"
#include "DrawableGameComponent.h"
#include <iostream>

// Standalone function to initialise the game.
void Init()
{
    std::cout << "Initialising game" << std::endl;
}

// Standalone function to terminate the game.
void Terminate()
{
    std::cout << "Terminating game" << std::endl;
}

int main()
{
    // Create a dynamic instance of the Game class with room for 2 components.
    Game *game = new Game(2);

    // Set the initialise and terminate functions.
    game->SetInitialise(Init);
    game->SetTerminate(Terminate);

    // Create a simple GameComponent with ID 1.
    GameComponent *component1 = new GameComponent();
    game->Add(component1);  // Add it to the game.

    // Create a DrawableGameComponent with ID 2 and starting position (0, 0).
    DrawableGameComponent *component2 = new DrawableGameComponent(0, 0);
    game->Add(component2);  // Add it to the game as well.

    // Run the game loop.
    game->Run();

    // Clean up memory after the game ends.
    delete game;
    delete component1;
    delete component2;

    return 0;
}
