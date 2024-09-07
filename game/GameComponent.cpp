#include "GameComponent.h"
#include <iostream>

// Static member to track how many instances are created.
int GameComponent::instances = 0;

// Constructor that assigns a unique ID to each GameComponent.
GameComponent::GameComponent()
{
    id = ++instances;
}

void GameComponent::Update(const tm *eventTime)
{
    // Print out the component's ID and the time of update.
    std::cout << "GameComponent ID: " << id << ", Time: "
              << eventTime->tm_hour << ":"
              << eventTime->tm_min << ":"
              << eventTime->tm_sec << std::endl;
}

int GameComponent::getId() const
{
    return id;  // Return the unique ID of this component.
}
