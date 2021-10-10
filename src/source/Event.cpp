//
// Created by johan on 12.01.2021.
//
#include "../headers/Event.hpp"

Event::Event(int timestamp) {
    this->timestamp = timestamp;
}

int Event::getTimestamp() {
    return this->timestamp;
}

EventType Event::getEventType() {
    return this->eventType;
}