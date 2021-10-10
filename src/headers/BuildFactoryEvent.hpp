//
// Created by stefa on 06.01.2021.
//
#include "Event.hpp"

#ifndef ADVPT_PROJECT_BUILDFACTORYEVENT_HPP
#define ADVPT_PROJECT_BUILDFACTORYEVENT_HPP

class BuildFactoryEvent : public Event {
    int factoryId;
    std::string factoryType;
    std::string factoryName;

public:
    BuildFactoryEvent(int timestamp, int factoryId, std::string factoryType, std::string factoryName);

    int getFactoryId();
    const std::string& getFactoryType();
    const std::string& getFactoryName();
};

#endif //ADVPT_PROJECT_BUILDFACTORYEVENT_HPP
