//
// Created by johan on 12.01.2021.
//

#include "../headers/StopFactoryEvent.hpp"

StopFactoryEvent::StopFactoryEvent(int factoryId, int timestamp) : Event(timestamp) {
    this->factoryId = factoryId;
    this->eventType = STOP_FACTORY;
}

int StopFactoryEvent::getFactoryId() {
    return this->factoryId;
}