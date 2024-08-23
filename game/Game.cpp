#include "Game.h"
#include <iostream>
#include <thread>
#include <chrono>

const int Game::TICKS_1000MS = 1000;

Game::Game(int maxComponents)
    : componentCount(0), components(new GameComponent *[maxComponents])
{
}

Game::~Game()
{
    delete[] components;
}

void Game::Add(GameComponent *component)
{
    components[componentCount++] = component;
}

void Game::SetInitialise(FP init)
{
    initialise = init;
}

void Game::SetTerminate(FP term)
{
    terminate = term;
}

void Game::Run()
{
    if (initialise)
        initialise();

    for (int i = 0; i < 5; i++)
    {
        time_t now = time(0);
        tm *eventTime = localtime(&now);

        for (int j = 0; j < componentCount; j++)
        {
            components[j]->Update(eventTime);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(TICKS_1000MS));
    }

    if (terminate)
        terminate();
}
