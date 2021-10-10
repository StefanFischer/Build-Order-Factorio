//
// Created by stefa on 06.01.2021.
//
#include <iostream>
#include "Event.hpp"

#ifndef ADVPT_PROJECT_STOPFACTORYEVENT_HPP
#define ADVPT_PROJECT_STOPFACTORYEVENT_HPP


class StopFactoryEvent : public Event {
    int factoryId;
public:
    StopFactoryEvent(int factoryId, int timestamp);

    int getFactoryId();
};

#endif //ADVPT_PROJECT_STOPFACTORYEVENT_HPP
