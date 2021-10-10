
#include "../headers/BuildFactoryEvent.hpp"

BuildFactoryEvent::BuildFactoryEvent(int timestamp, int factoryId, std::string factoryType, std::string factoryName)
        : Event::Event(timestamp) {
    this->factoryId = factoryId;
    this->factoryType = factoryType;
    this->factoryName = factoryName;
    this->eventType = BUILD_FACTORY;
}

int BuildFactoryEvent::getFactoryId() {
    return this->factoryId;
}

const std::string& BuildFactoryEvent::getFactoryType()
{
    return this->factoryType;
}

const std::string& BuildFactoryEvent::getFactoryName()
{
    return this->factoryName;
}