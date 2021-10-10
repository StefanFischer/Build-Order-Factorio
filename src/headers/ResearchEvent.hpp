//
// Created by stefa on 06.01.2021.
//
#include <fstream>
#include "Event.hpp"
#include "Technology.hpp"

#ifndef ADVPT_PROJECT_RESEARCHEVENT_HPP
#define ADVPT_PROJECT_RESEARCHEVENT_HPP


class ResearchEvent : public Event {
    Technology* technology;
public:
    ResearchEvent(Technology *technology, int timestamp);

    Technology* getTechnology();

    ~ResearchEvent();
};

#endif //ADVPT_PROJECT_RESEARCHEVENT_HPP
