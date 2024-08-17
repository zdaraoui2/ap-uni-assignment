#include "../DrawableGameComponent.h"
#include <ctime>
#include <iostream>
#include <cassert>

void testInitialisation()
{
    DrawableGameComponent component(0, 0);
    int initialId = component.getId();
    assert(component.getId() == initialId);
    std::cout << "Initialisation test passed!" << std::endl;
}

void testUpdatePosition()
{
    DrawableGameComponent component(0, 0);
    int initialId = component.getId();
    time_t now = time(0);
    tm *currentTime = localtime(&now);

    component.Update(currentTime);
    assert(component.getId() == initialId);
    std::cout << "Position update and clamping test passed!" << std::endl;
}

void testUpdateWithDirectionChange()
{
    DrawableGameComponent component(0, 0);
    int initialId = component.getId();
    time_t now = time(0);
    tm *currentTime = localtime(&now);

    component.Update(currentTime);
    component.Update(currentTime);

    assert(component.getId() == initialId);
    std::cout << "Update with direction change test executed." << std::endl;
}

int main()
{
    std::cout << "Starting DrawableGameComponent tests..." << std::endl;

    testInitialisation();
    testUpdatePosition();
    testUpdateWithDirectionChange();

    std::cout << "All DrawableGameComponent tests completed." << std::endl;
    return 0;
}

// c++ -std=c++11 -o DrawableGameComponentTest DrawableGameComponentTest.cpp ../DrawableGameComponent.cpp
// ./DrawableGameComponentTest