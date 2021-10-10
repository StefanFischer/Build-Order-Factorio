//
// Created by stefa on 06.01.2021.
//
#include <fstream>

#ifndef ADVPT_PROJECT_EVENT_HPP
#define ADVPT_PROJECT_EVENT_HPP
enum EventType {
    BUILD_FACTORY, DESTROY_FACTORY, START_FACTORY, STOP_FACTORY, RESEARCH_EVENT, VICTORY_EVENT
};

class Event {
protected:
    EventType eventType;
    int timestamp;
public:
    Event(int timestamp);

    int getTimestamp();

    EventType getEventType();
};

#endif //ADVPT_PROJECT_EVENT_HPP
