//
// Created by stefa on 06.01.2021.
//
#include <fstream>
#include <map>
#include <deque>
// json parser
#include <nlohmann_json.hpp>
// Game Entities
#include "Recipe.hpp"
#include "Utils.hpp"

#ifndef ADVPT_PROJECT_FACTORY_HPP
#define ADVPT_PROJECT_FACTORY_HPP


class Factory {
private:
    static int uniqueID;
    // list to keep all factoryIds which are available, after the original factory was deleted.
    // Take those IDs with higher priority
    static std::deque<int> availableFactoryIds;
    std::string name;
    double craftingSpeed;
    std::map<std::string, bool> craftingCategories;
    int factoryId;
    std::string factoryType;
    bool starved;
    bool justStarted;
    bool justFinished;

    // The recipe the factory is currently working on
    // = nullptr if the factory is inactive
    Recipe *currentRecipe;

    // Stores the point in time (in game-ticks) when the processing
    // of the current recipe will be done.
    // = -1 if the factory is inactive
    int gameTickWhenDoneProcessing;

    // get Id for next constructed Factory, takes care for Ids of deleted factories
    static int getNewFactoryId();

public:
    ~Factory();

    Factory(std::string name, std::string factoryType);

    Factory(std::string name, std::string factoryType, int factoryId);

    // Returns true, if the factory is currently processing
    bool isProcessing();

    // Returns true, if the factory is starved
    bool isStarved();

    // Returns true, if the factory has just been started
    bool getJustStarted();

    // Returns true, if the factory has just finished processing
    bool getJustFinished();

    // Returns the gameTick during which the factory finishes processing
    int getGameTickWhenDoneProcessing();

    // Sets the factory to starved state
    void setStarved();

    // Sets the justFinished flag
    void setJustFinished(bool val);

    // Returns the factories ID
    int getFactoryId();

    // Returns the getName of the factory
    const std::string& getFactoryName();

    // Returns the getName of the recipe, the factory is currently working on
    Recipe* getCurrentRecipe();

    // Initiates the starting of the factory. Does not actually start
    // it yet...
    void start(Recipe* recipe);

    // Starts the processing of the given recipe at the given time
    void startProcessingRecipe(int currentGameTick, Recipe* recipe);

    // Stops/Resets the factory
    // Be careful, information like the active recipe will be lost
    void stop();

    // get factory type
    std::string getFactoryType();

     std::map<std::string, bool> getCraftingCategories() const;

};

#endif //ADVPT_PROJECT_FACTORY_HPP
