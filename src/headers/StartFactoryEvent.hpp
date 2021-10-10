//
// Created by stefa on 06.01.2021.
//
#include <iostream>
#include "Event.hpp"
#include "Recipe.hpp"

#ifndef ADVPT_PROJECT_STARTFACTORYEVENT_HPP
#define ADVPT_PROJECT_STARTFACTORYEVENT_HPP



class StartFactoryEvent : public Event {
    int factoryId;
    Recipe *recipe;

public:
    StartFactoryEvent(int factoryId, Recipe *recipe, int timestamp);

    int getFactoryId();
    Recipe * getRecipe();
};

#endif //ADVPT_PROJECT_STARTFACTORYEVENT_HPP
