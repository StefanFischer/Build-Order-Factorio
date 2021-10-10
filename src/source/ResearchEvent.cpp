//
// Created by johan on 12.01.2021.
//

#include "../headers/ResearchEvent.hpp"

#include <utility>

ResearchEvent::ResearchEvent(Technology *parTechnology, int timestamp) :
        technology(parTechnology), Event(timestamp) {
    this->eventType = RESEARCH_EVENT;
}

Technology* ResearchEvent::getTechnology() {
    return technology;
}

ResearchEvent::~ResearchEvent() {
    delete technology;
}
