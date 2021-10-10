//
// Created by johan on 12.01.2021.
//

#include "../headers/VictoryEvent.hpp"

VictoryEvent::VictoryEvent(int timestamp) : Event(timestamp) {
    this->timestamp = timestamp;
    this->eventType = VICTORY_EVENT;
}