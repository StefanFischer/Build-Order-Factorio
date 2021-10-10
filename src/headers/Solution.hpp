//
// Created by johan on 06.02.2021.
//

#ifndef ADVPT_PROJECT_SOLUTION_HPP
#define ADVPT_PROJECT_SOLUTION_HPP

#include <nlohmann_json.hpp>
#include <list>
#include "tree.hh"
#include "tree_util.hh"
#include <iostream>

#include "Recipe.hpp"
#include "Technology.hpp"
#include "Factory.hpp"
#include "Event.hpp"
#include "GameEntity.hpp"

using json = nlohmann::json;

class Solution {
private:
    json technologyJson;
    json recipeJson;
    json factoryJson;
    json itemJson;
    json settingsJson;

    // Items player currently possesses
    std::map<std::string, int> currentItems;

    // Basis Items: (crafting category of player, no ingredients, enabled recipe)
    // Their respective recipe json name
    std::vector<std::string> basisItemsRecipeNames;

    // Crafting Categories player can process
    std::vector<std::string> playerCraftingCategories;

    // Recipes currently enabled
    std::vector<Recipe *> currentRecipes;
    std::vector<Recipe *> allRecipes;
    std::list<std::string> currentRecipeNames;
    std::map<std::string, tree<GameEntity*>> currentRecipeTreeMaps;

    // Technologies currently enabled
    std::vector<Technology> currentTechnologies;
    std::list<std::string> currentTechnologiesNames;
    std::map<std::string, tree<GameEntity*>> currentTechnologiesTreeMaps;
    // Factories currently built and processing a recipe
    std::list<Factory *> factories;

    // Goal Items to win game
    std::map<std::string, int> goalItems;

    // Currently Processed Events
    std::vector<Event *> processingEvents;

    // The build tree that is constructed to compute all required 
    // components
    tree<GameEntity*> buildTree;

    // search Recipe in Recipe-Json with given jsonKey
    Recipe* getRecipeByJsonKey(const std::string& jsonKey);

    // search first Recipe in Recipe-Json that cointains itemName in product-list
    Recipe* getRecipeByProductsList(const std::string& itemName);

    // search technology in Technology-Json with given jsonKey and return the needed energy
    int getTechnologEnergyByJsonKey(const std::string& technologyName);

    // search all Recipes in Recipe-Json that cointains itemName in product-list
    std::list<Recipe*> getAllPossibleRecipeByProductsList(const std::string& itemName);

    void DeleteBuildTree();

public:
    Solution(nlohmann::basic_json<> settingsJson, nlohmann::basic_json<> factoryJson, nlohmann::basic_json<> itemJson,
             nlohmann::basic_json<> recipeJson, nlohmann::basic_json<> technologyJson,
             std::map<std::string, int> currentItems, std::vector<Recipe *> currentRecipes,
             std::vector<Recipe *> allRecipes,
             std::vector<Technology> currentTechnologies, std::__cxx11::list<Factory *> factories,
             std::map<std::string, int> goalItems, std::vector<Event *> processingEvents);

    ~Solution();

    json getSolJson();

    void buildGoalItemRecipeTree();

    // Builds Tree of GameEntities and returns it
    // Leafs should be Basis Recipes (and their factory player)
    tree<GameEntity*> buildGoalItemGameEntityTree();

    // From GameEntity Tree construct the Event Build Order and return them as json
    json getBuildOrderFromTree(const tree<GameEntity*>& tr);

    // Computes a build tree that can be used to produce the goal item(s) currently given in the 
    // Solution's this->goalItems attribute.
    // This build tree is stored in the Solution's attribute this->buildTree.
    void computeBuildTree();

    // Finds all required game entities that are required to create the given game entity and adds
    // them to the given list.
    void GetRequirementsFromGameEntity(std::list<GameEntity*>& listOfReq, GameEntity* gameEntity, tree_node_<GameEntity*>* treeNode);

    // Finds all required game entities that are required to produce the given item in the given quantity
    // and adds them to the given list.
    void GetRequirementsForItem(std::list<GameEntity*>& listOfReq, const std::string& name, int numberOfInstances, tree_node_<GameEntity*>* treeNode);

    // Finds all required game entities that are required to build the given factory and adds them to the
    // given list.
    void GetRequirementsForFactory(std::list<GameEntity*>& listOfReq, const std::string& factoryType, tree_node_<GameEntity*>* treeNode);

    // Finds all required game entities that are required to unlock the given recipe and adds them to the 
    // given list.
    void GetRequirementsForRecipe(std::list<GameEntity*>& listOfReq, const std::string& recipeName, tree_node_<GameEntity*>* treeNode);

    // Finds all required game entities that are required to research the given technology and adds them 
    // to the given list.
    void GetRequirementsForTechnology(std::list<GameEntity*>& listOfReq, const std::string& techName, tree_node_<GameEntity*>* treeNode);

    // Find all basis items(crafting category of player, no ingredients, enabled recipe)
    // and all crafting categories that player can process and save them into member variables
    // basisItemsRecipeNames; playerCraftingCategories;
    void getBasisItemsRecipeNamesAndPlayerCategories();

    // Creates start and stop factory Events and returns them in json format
    std::tuple<json, json, unsigned long long> createFactoryEventsForRecipe(unsigned long long currentTimeStamp, tree_node_<GameEntity*> responsibleFactory, tree_node_<GameEntity*> recipeToProcess);

    //add technologies to tree
    tree<GameEntity*> addTechnologyProcessingToTree(tree<GameEntity*> tr, const std::list<tree_node_<GameEntity*>*>& reverseOrder);

    void addEnabledTechnologyBuild(const std::string& technologyName, GameEntity* requirements);
    tree<GameEntity*> copyEnabledTechnologyBuilds(const std::string& technologyName);
    bool technologyBuildKnown(const std::string& technologyName);


    bool recipeBuildKnown(const std::string& recipeName);
    void addEnabledRecipeBuild(const std::string& recipeName, GameEntity* recipe);
    tree<GameEntity*> copyEnabledRecipeBuilds(const std::string& recipeName, int amount);
};


#endif //ADVPT_PROJECT_SOLUTION_HPP
