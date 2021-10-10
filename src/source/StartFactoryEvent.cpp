//
// Created by johan on 12.01.2021.
//

#include "../headers/StartFactoryEvent.hpp"

StartFactoryEvent::StartFactoryEvent(int factoryId, Recipe * recipe, int timestamp) : Event(timestamp) {
    this->factoryId = factoryId;
    this->recipe = recipe;
    this->eventType = START_FACTORY;
}

int StartFactoryEvent::getFactoryId() {
    return this->factoryId;
}

Recipe * StartFactoryEvent::getRecipe()
{
    return this->recipe;
}