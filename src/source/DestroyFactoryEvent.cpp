//
// Created by johan on 12.01.2021.
//
#include "../headers/DestroyFactoryEvent.hpp"

DestroyFactoryEvent::DestroyFactoryEvent(int factoryId, int timestamp) : Event::Event(timestamp) {
    this->factoryId = factoryId;
    this->eventType = DESTROY_FACTORY;
}

int DestroyFactoryEvent::getFactoryId() {
    return this->factoryId;
}