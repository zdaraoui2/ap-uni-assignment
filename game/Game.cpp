#include "Game.h"
#include <iostream>
#include <thread>
#include <chrono>

// How long each tick should last (1 second).
const int Game::TICKS_1000MS = 1000;

// Constructor that sets up the component array.
Game::Game(int maxComponents)
    : componentCount(0), components(new GameComponent *[maxComponents]) {}

Game::~Game()
{
    // Clean up memory by deleting the array of components.
    delete[] components;
}

void Game::Add(GameComponent *component)
{
    // Add a new component to the game and increment the count.
    components[componentCount++] = component;
}

void Game::SetInitialise(FP init)
{
    initialise = init;  // Set the initialise function.
}

void Game::SetTerminate(FP term)
{
    terminate = term;  // Set the terminate function.
}

void Game::Run()
{
    // Call the initialise function if it's been set.
    if (initialise)
        initialise();

    // Main game loop, runs 5 times with a 1-second delay between each update.
    for (int i = 0; i < 5; i++)
    {
        // Get the current time.
        time_t now = time(0);
        tm *eventTime = localtime(&now);

        // Update all components.
        for (int j = 0; j < componentCount; j++)
        {
            components[j]->Update(eventTime);
        }

        // Pause for 1 second before the next update.
        std::this_thread::sleep_for(std::chrono::milliseconds(TICKS_1000MS));
    }

    // Call the terminate function if it's been set.
    if (terminate)
        terminate();
}
