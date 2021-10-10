//
// Created by johan on 12.01.2021.
//
#include "../headers/Factory.hpp"


using json = nlohmann::json;


int Factory::uniqueID = 0;
std::deque<int> Factory::availableFactoryIds;

Factory::Factory(std::string name, std::string factoryType) : Factory(name, factoryType, getNewFactoryId()) {
    // computes new factoryID

}


Factory::Factory(std::string name, std::string factoryType, int factoryId) {
    this->name = name;
    this->factoryId = factoryId;
    this->starved = true;
    this->justStarted = false;
    this->justFinished = false;
    this->currentRecipe = nullptr;
    this->gameTickWhenDoneProcessing = -1;
// get crafting categories from factoryType

// load Factory json file
    std::ifstream factoriesStream(g_factoryJsonPath);

// read json file
    json factoriesSettings = json::parse(factoriesStream);

// get Factory crafting speed and categories from Factory type
    auto factorySpecification = factoriesSettings.find(factoryType);
    this->craftingSpeed = *factorySpecification->find("crafting_speed");
    auto craftingCategories = factorySpecification->find("crafting_categories");
    std::map<std::string, bool> categoriesMap = craftingCategories->get<std::map<std::string, bool >>();
    this->craftingCategories = categoriesMap;
}

Factory::~Factory() {
    Factory::availableFactoryIds.push_back(this->factoryId);
}


int Factory::getNewFactoryId() {
    // check if availableFactoryIds is empty
    int newId = 0;
    if (!availableFactoryIds.empty()) {
        // if there are available Ids take those
        newId = availableFactoryIds.front();
        availableFactoryIds.pop_front();
    } else {
        newId = uniqueID;
        uniqueID++;
    }
    return newId;
}

bool Factory::isProcessing() {
    return this->gameTickWhenDoneProcessing > 0;
}

void Factory::setStarved() {
    this->starved = true;
    this->justStarted = false;
    this->justFinished = false;
    this->gameTickWhenDoneProcessing = -1;
}

void Factory::setJustFinished(bool val) {
    this->justFinished = val;
}

bool Factory::isStarved() {
    return this->starved;
}

bool Factory::getJustStarted() {
    return this->justStarted;
}

bool Factory::getJustFinished() {
    return this->justFinished;
}

int Factory::getGameTickWhenDoneProcessing() {
    return this->gameTickWhenDoneProcessing;
}

int Factory::getFactoryId() {
    return this->factoryId;
}

const std::string &Factory::getFactoryName() {
    return this->name;
}

Recipe* Factory::getCurrentRecipe() {
    return this->currentRecipe;
}

void Factory::start(Recipe *recipe) {
    this->justStarted = true;
    this->currentRecipe = recipe;
}

void Factory::startProcessingRecipe(int currentGameTick, Recipe *recipe) {
    this->starved = false;
    this->justStarted = false;
    this->justFinished = false;
    this->gameTickWhenDoneProcessing =
            currentGameTick + std::ceil(recipe->getEnergy() / this->craftingSpeed);
}

void Factory::stop() {
    this->currentRecipe = nullptr;
    this->starved = false;
    this->justStarted = false;
    this->justFinished = false;
    this->gameTickWhenDoneProcessing = -1;
}

std::string Factory::getFactoryType(){
    return this->factoryType;
}

std::map<std::string, bool> Factory::getCraftingCategories() const{
    return craftingCategories;
}