//
// Created by stefa on 06.01.2021.
//
#include "Event.hpp"

#ifndef ADVPT_PROJECT_DESTROYFACTORYEVENT_HPP
#define ADVPT_PROJECT_DESTROYFACTORYEVENT_HPP

class DestroyFactoryEvent : public Event {
    int factoryId;

public:
    DestroyFactoryEvent(int factoryId, int timestamp);

    int getFactoryId();
};

#endif //ADVPT_PROJECT_DESTROYFACTORYEVENT_HPP
