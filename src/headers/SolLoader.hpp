//
// Created by johan on 28.01.2021.
//
#include <iostream>
#include <list>
// json parser
#include <nlohmann_json.hpp>

using json = nlohmann::json;

#include "Solution.hpp"
// Game Entities
#include "Recipe.hpp"
#include "Factory.hpp"
#include "Technology.hpp"

// Game Events
#include "Event.hpp"

#include "Utils.hpp"

#ifndef ADVPT_PROJECT_SIMLOADER_H
#define ADVPT_PROJECT_SIMLOADER_H


class SolLoader {
private:
    json technologyJson;
    json recipeJson;
    json factoryJson;
    json itemJson;
    json settingsJson;

    // Items player currently possesses
    std::map<std::string, int> currentItems;
    std::map<std::string, int> allItems;

    // Recipes currently enabled
    std::vector<Recipe *> currentRecipes;
    std::vector<Recipe *> allRecipes;

    // Technologies currently enabled
    std::vector<Technology> currentTechnologies;
    std::vector<Technology> allTechnologies;

    // Factories currently built and processing a recipe
    std::list<Factory *> factories;
    std::vector<Factory *> allFactories;

    // Goal Items to win game
    std::map<std::string, int> goalItems;

    // Currently Processed Events
    std::vector<Event *> processingEvents;


    void init(std::string &challengePath);

    Factory *getFactoryFromJson(const std::string &factoryName);

    Technology *getTechnologyFromJson(const std::string &name);

    Recipe *getRecipeFromJson(const std::string &name);

    static std::map<std::string, int> getIngredients(json &ingredientsJson);

public:
    std::unique_ptr<Solution> createSolution(std::string &challengePath);
};


#endif //ADVPT_PROJECT_SIMLOADER_H
