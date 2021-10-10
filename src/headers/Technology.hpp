//
// Created by stefa on 06.01.2021.
//
#include <fstream>
#include <map>
#include <vector>
#include "Recipe.hpp"

#ifndef ADVPT_PROJECT_TECHNOLOGY_HPP
#define ADVPT_PROJECT_TECHNOLOGY_HPP


class Technology {
    const std::string name;
    const std::vector<std::string> prerequisites;
    const std::vector<Recipe *> recipes;
    const std::map<std::string, int> ingredients;

public:
    Technology(std::string name, std::vector<std::string> prerequisites, std::vector<Recipe *> recipes,
               std::map<std::string, int> ingredients);

    std::string getName();

    bool checkPrerequisites(const std::vector<std::string> &technologies);

    /**
    * executes the technology. Alters the given map to the new state.
    * Does not alter map if execution fails.
    * @return false if the technology could not be executed due to missing items
    */
    bool execute(std::map<std::string, int> &itemsAvailable, std::vector<Technology> &curTech);

    std::vector<Recipe *> getRecipes();

    bool operator==(const Technology &tech) const;

    bool operator<(const Technology &tech) const;

    bool operator>(const Technology &tech) const;

    //TODO overload stream operator instead of java toString method ???
    std::string toString();

    ~Technology();

};

#endif //ADVPT_PROJECT_TECHNOLOGY_HPP
