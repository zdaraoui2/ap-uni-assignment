#include "../Game.h"
#include "../DrawableGameComponent.h"
#include <ctime>
#include <iostream>
#include <cassert>

void testInitialisation()
{
    Game game(2);
    std::cout << "Game initialisation test passed!" << std::endl;
}

void testAddComponents()
{
    Game game(2);
    DrawableGameComponent component1(0, 0);
    DrawableGameComponent component2(10, 10);

    game.Add(&component1);
    game.Add(&component2);

    std::cout << "Add components test passed!" << std::endl;
}

void testRunGame()
{
    Game game(2);

    game.SetInitialise([]()
                       { std::cout << "Game is starting..." << std::endl; });
    game.SetTerminate([]()
                      { std::cout << "Game is ending..." << std::endl; });

    DrawableGameComponent component1(0, 0);
    DrawableGameComponent component2(10, 10);

    game.Add(&component1);
    game.Add(&component2);

    game.Run();

    std::cout << "Run game test executed, check output manually." << std::endl;
}

int main()
{
    std::cout << "Starting Game tests..." << std::endl;

    testInitialisation();
    testAddComponents();
    testRunGame();

    std::cout << "All Game tests completed." << std::endl;
    return 0;
}

// c++ -std=c++11 -o GameTest GameTest.cpp ../Game.cpp ../GameComponent.cpp ../DrawableGameComponent.cpp
// ./GameTest
