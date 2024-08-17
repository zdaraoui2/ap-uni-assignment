#include "../GameComponent.h"
#include <ctime>
#include <iostream>
#include <thread>
#include <chrono>

int main()
{
    GameComponent component1;
    GameComponent component2;
    GameComponent component3;

    time_t now = time(0);
    tm *currentTime = localtime(&now);
    component1.Update(currentTime);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    now = time(0);
    currentTime = localtime(&now);
    component2.Update(currentTime);

    std::this_thread::sleep_for(std::chrono::seconds(2));
    now = time(0);
    currentTime = localtime(&now);
    component3.Update(currentTime);

    return 0;
}

// c++ -std=c++11 -o GameComponentTest GameComponentTest.cpp ../GameComponent.cpp
// ./GameComponentTest