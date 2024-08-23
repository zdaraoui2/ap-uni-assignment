#include "GameComponent.h"
#include <iostream>

int GameComponent::instances = 0;

GameComponent::GameComponent()
{
    id = ++instances;
}

void GameComponent::Update(const tm *eventTime)
{
    std::cout << "GameComponent ID: " << id << ", Time: "
              << eventTime->tm_hour << ":"
              << eventTime->tm_min << ":"
              << eventTime->tm_sec << std::endl;
}

int GameComponent::getId() const
{
    return id;
}
