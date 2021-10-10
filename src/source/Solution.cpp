//
// Created by johan on 06.02.2021.
//

#include <stdexcept>
#include <algorithm>
#include <iomanip>

#include "../headers/Solution.hpp"

Solution::Solution(nlohmann::basic_json<> settingsJson, nlohmann::basic_json<> factoryJson,
                   nlohmann::basic_json<> itemJson, nlohmann::basic_json<> recipeJson,
                   nlohmann::basic_json<> technologyJson, std::map<std::string, int> currentItems,
                   std::vector<Recipe *> currentRecipes, std::vector<Recipe *> allRecipes,
                   std::vector<Technology> currentTechnologies,
                   std::__cxx11::list<Factory *> factories, std::map<std::string, int> goalItems,
                   std::vector<Event *> processingEvents) : settingsJson(std::move(settingsJson)), factoryJson(std::move(factoryJson)),
                                                            itemJson(std::move(itemJson)), recipeJson(std::move(recipeJson)),
                                                            technologyJson(std::move(technologyJson)), currentItems(std::move(currentItems)),
                                                            currentRecipes(std::move(currentRecipes)), allRecipes(std::move(allRecipes)),
                                                            currentTechnologies(std::move(currentTechnologies)),
                                                            factories(std::move(factories)), goalItems(std::move(goalItems)),
                                                            processingEvents(std::move(processingEvents)) {
    getBasisItemsRecipeNamesAndPlayerCategories();
    currentTechnologiesNames = std::list<std::string>();

}

// use already researched technologies again by copying their build tree: optimizes build tree creation but not printing
bool copyKnownTechnologies = false;
bool copyKnownRecipes = false;
// if false do not technology processing in first step but after
bool doTechnologyProcessing = true;

Solution::~Solution()
{
    DeleteBuildTree();
}

void print_GameEntity_Tree(const tree<GameEntity*>& tr)
{
    tree<GameEntity*>::pre_order_iterator it = tr.begin();
    tree<GameEntity*>::pre_order_iterator end = tr.end();
    if(!tr.is_valid(it)) return;
    int rootdepth=tr.depth(it);
    std::cout << "-----" << std::endl;
    while(it!=end) {
        for(int i=0; i<tr.depth(it)-rootdepth; ++i)
            std::cout << "  ";
        std::cout << it.operator*()->getGameEntityString() << " : " <<(it.operator*()->GetIdentifier())
                  << " , #=" << it.operator*()->GetNumberOfInstances()
                  << " , Factory ID = " << it.operator*()->getFactoryID() << std::endl << std::flush;
        ++it;
    }
    std::cout << "-----" << std::endl;
}


Recipe* Solution::getRecipeByJsonKey(const std::string& jsonKey) {
    for (auto recipe : allRecipes) {
        if (recipe->getName() == jsonKey) {
            return recipe;
        }
    }
    return nullptr;
}

Recipe* Solution::getRecipeByProductsList(const std::string& itemName) {
    for (auto recipe : allRecipes) {
        for (const auto& products : recipe->getProducts()) {
            if (products.first == itemName) {
                return recipe;
            }
        }
    }
    return nullptr;
}

int Solution::getTechnologEnergyByJsonKey(const std::string& technologyName) {
    json tech = technologyJson[technologyName];
    std::cout << std::setw(4) << tech << std::endl;
    int energy = tech["energy"].get<int>();
    return energy;
}

std::list<Recipe*> Solution::getAllPossibleRecipeByProductsList(const std::string& itemName) {
    std::list<Recipe*> possibleRecipes;
    for (auto recipe : allRecipes) {
        for (const auto& products : recipe->getProducts()) {
            if (products.first == itemName) {
                possibleRecipes.push_back(recipe);
            }
        }
    }
    return possibleRecipes;
}

tree<GameEntity*> Solution::buildGoalItemGameEntityTree() {
    tree<GameEntity*> tr;
    tree<GameEntity*>::iterator root;


    for (const auto& item : goalItems) {
        std::string itemName = item.first;
        //std::cout << "goal-item : " << itemName << std::endl;

        // add now the root goal item entity
        GameEntity* goalItem = new GameEntity(ITEM, itemName, item.second, root.node);
        tr.insert(root, goalItem);


        //tree<GameEntity*>::breadth_first_iterator start = tr.begin_breadth_first();
        tree<GameEntity*>::iterator start = tr.begin();
        //while(start!=tr.end_breadth_first()) {
        while(start!=tr.end()) {
            GameEntity* nextEntity = start.operator*();
            if (copyKnownTechnologies) {
                if (nextEntity->GetTypeOfGameEntity() == TECHNOLOGY) {
                    if (technologyBuildKnown(nextEntity->GetIdentifier())) {
                        //std::cout << "copy technology: " << nextEntity->GetIdentifier() << std::endl;
                        auto it = copyEnabledTechnologyBuilds(nextEntity->GetIdentifier());
                        tr.replace(start, it.begin());
                        //tr.insert_subtree(start, it.begin());
                        ++start;
                        continue;
                    } else {
                        //std::cout << "new technology: " << nextEntity->GetIdentifier() << std::endl;
                        addEnabledTechnologyBuild(nextEntity->GetIdentifier(), start.node->data);
                        auto it = copyEnabledTechnologyBuilds(nextEntity->GetIdentifier());
                        tr.replace(start, it.begin());
                        //tr.insert_subtree(start, it.begin());
                        ++start;
                        continue;
                    }
                }
            }
            if (copyKnownRecipes) {
                if (nextEntity->GetTypeOfGameEntity() == RECIPE) {
                    int amount = nextEntity->GetNumberOfInstances();
                    if (recipeBuildKnown(nextEntity->GetIdentifier())) {
                        std::cout << "old recipe: " << nextEntity->GetIdentifier() << " : real amount " << nextEntity->GetNumberOfInstances() << std::endl;
                        auto it = copyEnabledRecipeBuilds(nextEntity->GetIdentifier(), amount);
                        tr.replace(start, it.begin());
                        //tr.insert_subtree(start, it.begin());
                        ++start;
                        continue;
                    } else {
                        addEnabledRecipeBuild(nextEntity->GetIdentifier(), start.node->data);
                        std::cout << "new recipe: " << nextEntity->GetIdentifier() << " : " << nextEntity->GetNumberOfInstances() << std::endl;
                        auto it = copyEnabledRecipeBuilds(nextEntity->GetIdentifier(), amount);
                        tr.replace(start, it.begin());
                        //tr.insert_subtree(start, it.begin());
                        //std::cout << "first insertion done" << std::endl;
                        ++start;
                        //print_GameEntity_Tree(tr);
                        continue;
                    }
                }
            }

            // get all requirements
            std::list<GameEntity*> listOfReq;
            this->GetRequirementsFromGameEntity(listOfReq, nextEntity, start.node);
            for (auto req : listOfReq) {
                // add all requirements as child to current game entity node
                tr.append_child(start, req);
            }
            // iterate to next node with bfs
            ++start;
            //print_GameEntity_Tree(tr);
        }
    }
    //print_GameEntity_Tree(tr);

    return tr;
}

void Solution::GetRequirementsFromGameEntity(std::list<GameEntity*>& listOfReq, GameEntity* gameEntity, tree_node_<GameEntity*>* parentTreeNode)
{
    switch(gameEntity->GetTypeOfGameEntity())
    {
        case TypeOfGameEntity::ITEM:
            GetRequirementsForItem(listOfReq,
                                   gameEntity->GetIdentifier(),
                                   gameEntity->GetNumberOfInstances(),
                                   parentTreeNode);
            break;
        case TypeOfGameEntity::FACTORY:
            GetRequirementsForFactory(listOfReq,
                                      gameEntity->GetIdentifier(),
                                      parentTreeNode);
            break;
        case TypeOfGameEntity::RECIPE:
            GetRequirementsForRecipe(listOfReq,
                                     gameEntity->GetIdentifier(),
                                     parentTreeNode);
            break;
        case TypeOfGameEntity::TECHNOLOGY:
            GetRequirementsForTechnology(listOfReq,
                                         gameEntity->GetIdentifier(),
                                         parentTreeNode);
            break;
        default: throw std::invalid_argument("Unknown type of Game Entity");
    }
}

bool isRecipeLoopInTree(const std::string& recipeName, tree_node_<GameEntity*>* node) {
    while(node->parent != nullptr) {
        // go till root
        if (node->data->GetTypeOfGameEntity() == RECIPE) {
            // current node is RECIPE node
            if (node->data->GetIdentifier() == recipeName) {
                // current RECIPE is already contained in path from root to node
                // therefore this would cause a loop -> do not add it
                std::cout << "found loop for Recipe: " << recipeName << std::endl;
                return true;
            }
        }
        node = node->parent;
    }
    return false;
}

bool isTechnologyLoopInTree(const std::string& technologyName, tree_node_<GameEntity*>* node) {
    while(node->parent != nullptr) {
        // go till root
        if (node->data->GetTypeOfGameEntity() == TECHNOLOGY) {
            // current node is TECHNOLOGY node
            if (node->data->GetIdentifier() == technologyName) {
                // current TECHNOLOGY is already contained in path from root to node
                // therefore this would cause a loop -> do not add it
                std::cout << "found loop for Technology: " << technologyName << std::endl;
                return true;
            }
        }
        node = node->parent;
    }
    return false;
}

void Solution::GetRequirementsForItem(std::list<GameEntity*>& listOfReq, const std::string& name, int numberOfInstances, tree_node_<GameEntity*>* parentTreeNode)
{
    // Find a recipe in recipe.json where the product is this item
    // - If the desired item is contained in the ingredients as well, skip that recipe
    //     Create a new GameEntity(...) for this recipe and add it to the given list
    std::list<Recipe*> possibleRecipes = this->getAllPossibleRecipeByProductsList(name);
    // remove all barrel related recipes
    //TODO weird fix for weird problem (only debugged for openmp so maybe overkill)
    int size = possibleRecipes.size();
    auto it = possibleRecipes.begin();
    for (int i = 0; i < size; ++i, ++it){
        auto recipe = it.operator*();
        if (recipe->getName().find("barrel") != std::string::npos) {
            possibleRecipes.remove(recipe);
            --size;
        }
    }

    Recipe* nextRecipe = possibleRecipes.front();

    // If the desired item is contained in the ingredients as well, remove that recipe
    while (nextRecipe->getIngredients().find(name) != nextRecipe->getIngredients().end()) {
        // desired item is again contained in the ingredients itself
        possibleRecipes.pop_front();
        nextRecipe = possibleRecipes.front();
    }

    // Now we have all recipes in possibleRecipes that are not self-containing

    // How to find low-level recipes???
    // Heuristic: low level recipes are already enabled
    // Heuristic: basic, normal, advanced in crafting-category names
    bool foundEasyVersion = false;
    for (auto recipe : possibleRecipes) {
        if (recipe->getCategory().find("basic") != std::string::npos) {
            // it is a basic-recipe and enabled
            if (recipe->isEnabled()) {
                nextRecipe = recipe;
                foundEasyVersion = true;
                break;
            }
        }
    }
    if (!foundEasyVersion) {
        for (auto recipe : possibleRecipes) {
            // it is enabled, therefore easy
            if (recipe->isEnabled()) {
                nextRecipe = recipe;
                foundEasyVersion = true;
                break;
            }
        }
    }
    // for fluid processing the recipes name are also containing basic and advanced
    for (auto recipe : possibleRecipes) {
        if (recipe->getName().find("basic") != std::string::npos) {
            // it is a basic-recipe and enabled
            nextRecipe = recipe;
            foundEasyVersion = true;
            break;
        }
    }

    // Do backtracking to prevent loops
    auto node = parentTreeNode;
    if (isRecipeLoopInTree(nextRecipe->getName(), node)) {
        std::cout << "Heuristic enforced recipe is loop, select alternatives on same tree level: " << name <<  std::endl;
        nextRecipe = possibleRecipes.front();
    }

    while (isRecipeLoopInTree(nextRecipe->getName(), node)) {
        possibleRecipes.pop_front();
        if (possibleRecipes.empty()) {
            std::cout << "No recipes left to choose from on same tree level, we took wrong choice already in predecessors: " << name << std::endl;
        }
        nextRecipe = possibleRecipes.front();
    }



    // By the needed number of instances of this item and the number of instances produced in the recipe,
    // calculate how many times the recipe has to be executed.
    //     Create new GameEntity(...) for each required item type (recipe.ingredient) and insert the number of
    //     instances. Add this GameEntity to the list
    int numberOfInstancesProducing = 0;
    auto products = nextRecipe->getProducts();
    for (const auto& p : products) {
        if (p.first == name) {
            numberOfInstancesProducing = p.second;
        }
    }

    // how often recipe has to be processed for amount of needed items
    int numberOfProcessingRecipe = std::ceil(( (double) numberOfInstances / (double) numberOfInstancesProducing));
    if (numberOfProcessingRecipe == 0) {
        // numerical instabilies (no clue how to fix it else)
        numberOfProcessingRecipe = 1;
    }

    // Find a factory that provides the crafting-category that is required by the recipe.
    possibleRecipes.remove(nextRecipe);
    GameEntity* entity = new GameEntity(RECIPE, nextRecipe->getName(), numberOfProcessingRecipe, parentTreeNode, possibleRecipes);
    listOfReq.push_back(entity);
}

void Solution::GetRequirementsForFactory(std::list<GameEntity*>& listOfReq, const std::string& factoryType, tree_node_<GameEntity*>* parentTreeNode)
{
    // Find Item which is the factory we need
    // if factory is player then no requirements needed
    if (factoryType == "player") {
        return;
    }

    // we only need one factory
    GameEntity* entity = new GameEntity(ITEM, factoryType, 1, parentTreeNode);
    listOfReq.push_back(entity);
}

void Solution::GetRequirementsForRecipe(std::list<GameEntity*>& listOfReq, const std::string& recipeName, tree_node_<GameEntity*>* parentTreeNode)
{
    // recipe needs:    category -> factory
    //                  enabled -> technology
    //                  ingredients -> items
    // TODO: jedes recipe bekommt eigene factory??? kann das simple verbessert werden? Möglich: GameEntity
    // TODO: Recipe sollte auch numberOfInstances haben

    // #### find a factory that can process this recipe: correct crafting category

    // start by finding recipe and the needed factory category
    Recipe* recipe = getRecipeByJsonKey(recipeName);
    std::string categoryRecipe = recipe->getCategory();

    // #### check if this recipe is basis-recipe and can therefore be crafted by player
    bool foundFactory = false;
    if (std::find(this->basisItemsRecipeNames.begin(), this->basisItemsRecipeNames.end(), recipeName)!= this->basisItemsRecipeNames.end()) {
        // we need no requirements
        GameEntity* entity = new GameEntity(FACTORY, "player", 1, parentTreeNode);
        listOfReq.push_back(entity);
        foundFactory = true;
    } else if (recipe->isEnabled() &&
               std::find(this->playerCraftingCategories.begin(), this->playerCraftingCategories.end(), categoryRecipe)!= this->playerCraftingCategories.end()) {
        // if recipe can be processed by player and is enabled it can also be processed
        GameEntity* entity = new GameEntity(FACTORY, "player", 1, parentTreeNode);
        listOfReq.push_back(entity);
        foundFactory = true;
    } else if (std::find(this->playerCraftingCategories.begin(), this->playerCraftingCategories.end(), categoryRecipe)!= this->playerCraftingCategories.end()) {
        // if recipe can be processed by player
        GameEntity *entity = new GameEntity(FACTORY, "player", 1, parentTreeNode);
        listOfReq.push_back(entity);
        foundFactory = true;
    }

    // find factory name by factory category
    // TODO: Caution here multiple factory types are possible!!!
    // TODO: therefore we introduce sorting to factories: sort by crafting speed (Assumption: lower speeds -> lower requirements)
    if (foundFactory == false) {
        std::string neededFactoryType;
        double minCraftingSpeed = 10;
        std::string lowestLevelFactory;
        for (auto it = factoryJson.begin(); it != factoryJson.end(); ++it) {
            const auto &factoryType = it.key();
            auto list = it.value();
            auto craftingCategories = list["crafting_categories"].get<std::map<std::string, bool>>();
            for (const auto& category : craftingCategories) {
                if(category.first == categoryRecipe){
                    neededFactoryType = factoryType;
                    double factoryCraftingSpeed = list["crafting_speed"].get<double>();
                    if (factoryCraftingSpeed < minCraftingSpeed) {
                        // take factory with lowest crafting speed
                        minCraftingSpeed = factoryCraftingSpeed;
                        lowestLevelFactory = factoryType;
                    }
                }
            }
        }

        // needs one factory
        GameEntity* entity = new GameEntity(FACTORY, lowestLevelFactory, 1, parentTreeNode);
        listOfReq.push_back(entity);
    }
    //done factory selection

    // #### continue with needed technology, only if recipe is not enabled from start

    if (!recipe->isEnabled()) {
        // recipe not enabled from start search for technology enabling recipe
        std::string technologyName;
        bool found = false;
        for (auto it = technologyJson.begin(); it != technologyJson.end(); it++) {
            auto list = it.value();
            auto effects = list["effects"];
            std::string unlockedRecipe;
            for (json::iterator it2 = effects.begin(); it2 != effects.end(); ++it2) {
                // search through all technologies to find if effect unlocks recipe
                auto val = it2.value();
                unlockedRecipe = val.at("recipe");

                if (unlockedRecipe == recipeName) {
                    // we have found the technolgy to the recipe
                    technologyName = it.key();
                    found = true;
                    break;
                }
            }
            if (found) {
                break;
            }
        }

        GameEntity* entity = new GameEntity(TECHNOLOGY, technologyName, 1, parentTreeNode);
        listOfReq.push_back(entity);
    }

    // #### now find corresponding ingredient items and add them to list of requirements
    // check how often recipe has to be processed
    int numberOfProcessings = parentTreeNode->data->GetNumberOfInstances();
    auto ingredients = recipe->getIngredients();
    for (const auto& i: ingredients) {
        GameEntity* entity = new GameEntity(ITEM, i.first, i.second * numberOfProcessings, parentTreeNode);
        listOfReq.push_back(entity);
    }
}

void Solution::GetRequirementsForTechnology(std::list<GameEntity*>& listOfReq, const std::string& techName, tree_node_<GameEntity*>* parentTreeNode)
{
    if (doTechnologyProcessing) {
        // needs:   ingredients-> items
        //          prerequisities-> other technologies

        // #### get technology by name and add ingredients to list of requisities
        auto technology = technologyJson[techName];

        auto ingredients = technology["ingredients"];
        for (json::iterator it = ingredients.begin(); it != ingredients.end(); ++it) {
            auto tuple = it.value();
            std::string name = tuple["name"];
            int amount = tuple["amount"];
            GameEntity* entity = new GameEntity(ITEM, name, amount, parentTreeNode);
            listOfReq.push_back(entity);
        }

        // #### get prerequisities-> other technologies
        auto prerequisities = technology["prerequisites"].get<std::list<std::string>>();
        for (const auto& p : prerequisities) {
            if (isTechnologyLoopInTree(p, parentTreeNode)) {
                std::cout << "Loop in Technologies" << std::endl;
            }
            GameEntity* entity = new GameEntity(TECHNOLOGY, p, 1, parentTreeNode);
            listOfReq.push_back(entity);
        }

    }
}

void Solution::getBasisItemsRecipeNamesAndPlayerCategories() {
    // Find factory "player"s crafting-categories
    json playerJson = factoryJson["player"];
    auto playerCategoriesMap = playerJson["crafting_categories"].get<std::map<std::string, bool>>();
    std::vector<std::string> playerCategories;
    for (const auto& category : playerCategoriesMap) {
        // category.first is holding the string name of category type
        playerCategories.push_back(category.first);
    }
    this->playerCraftingCategories = playerCategories;

    std::vector<std::string> basisItems;
    for (Recipe* recipe : allRecipes) {
        // check requirements for basis recipe/item
        // 1. crafting category of player
        // 2. no ingredients
        // 3. enabled recipe

        if (recipe->isEnabled() && recipe->getIngredients().empty()) {
            std::string category = recipe->getCategory();
            bool isInPlayerCategories = std::find(std::begin(playerCategories), std::end(playerCategories), category) != playerCategories.end();
            if (isInPlayerCategories) {
                // json key of recipe to basis items list
                basisItems.push_back(recipe->getName());
                //std::cout << "base item: " << recipe->getName() << std::endl;
            }
        }
    }
    this->basisItemsRecipeNames = basisItems;
    return;
}

void Solution::DeleteBuildTree()
{
    tree<GameEntity*>::iterator it = this->buildTree.begin();
    tree<GameEntity*>::iterator end = this->buildTree.end();
    while(it != end)
    {
        delete *it;
        ++it;
    }
}

std::tuple<json, json, unsigned long long> Solution::createFactoryEventsForRecipe(unsigned long long currentTimeStamp, tree_node_<GameEntity*> responsibleFactory, tree_node_<GameEntity*> recipeToProcess) {
    // two events: start factory event and stop factory event


    auto factory =  responsibleFactory.data;
    auto recipe = recipeToProcess.data;

    // create start factory event
    json startFactoryEvent;
    startFactoryEvent["type"] = "start-factory-event";
    startFactoryEvent["timestamp"] = currentTimeStamp;
    startFactoryEvent["factory-id"] = factory->getFactoryID();
    startFactoryEvent["recipe"] = recipe->GetIdentifier();


    // create stop factory event
    json stopFactoryEvent;
    stopFactoryEvent["type"] = "stop-factory-event";
    // compute timestamp of stop event: current time stamp + duration of one recipe * numberOfNeededItems
    Recipe* recipeAsRecipe = this->getRecipeByJsonKey(recipe->GetIdentifier());
    json factoryDescription = factoryJson[factory->GetIdentifier()];
    double craftingSpeed = factoryDescription["crafting_speed"];
    double energyForOneProcessing = recipeAsRecipe->getEnergy();
    double timeForOneRecipeProcessing = std::ceil(((double) energyForOneProcessing / (double) craftingSpeed));
    //std::cout << "Time needed for one recept: " << timeForOneRecipeProcessing << std::endl;
    //std::cout << "Energy needed for one recept: " << energyForOneProcessing << std::endl;
    //::cout << "Factory Crafting Speed: " << craftingSpeed << std::endl;
    std::string itemNeeded = recipeToProcess.parent->data->GetIdentifier();
    //std::cout << "ITEM needed: " << recipeToProcess.parent->data->GetIdentifier() << ", amount=" << recipeToProcess.parent->data->GetNumberOfInstances() << std::endl;
    //std::cout << "RECIPE used: " << recipeToProcess.parent->data->GetIdentifier() << ", processedNumber=" << recipeToProcess.data->GetNumberOfInstances() << std::endl;
    int itemsPerProcessing = recipeAsRecipe->getProducts().at(itemNeeded);
    //int numberOfProcessings = recipeToProcess.parent->data->GetNumberOfInstances();
    double numberOfProcessings = recipeToProcess.data->GetNumberOfInstances();
    unsigned long long stopTimeStamp = std::ceil((double) currentTimeStamp + (timeForOneRecipeProcessing * numberOfProcessings) *2);
    stopFactoryEvent["timestamp"] = stopTimeStamp;
    stopFactoryEvent["factory-id"] = factory->getFactoryID();
    json bothJsons = {startFactoryEvent, stopFactoryEvent};
    //std::cout << std::setw(4) << bothJsons << std::endl;
    return std::make_tuple(startFactoryEvent, stopFactoryEvent, stopTimeStamp);
}

tree<GameEntity*> Solution::addTechnologyProcessingToTree(tree<GameEntity*> tr, const std::list<tree_node_<GameEntity*>*>& reverseOrder) {
    // find last (cause bfs starts with last elements) occurence of technologies and from that add requirements
    std::map<std::string, int> lastOccurence;
    tree<GameEntity*>::breadth_first_iterator bfs = tr.begin_breadth_first();
    while(bfs!=tr.end_breadth_first()) {
        if (bfs.node->data->GetTypeOfGameEntity() == TECHNOLOGY) {
            auto pos = lastOccurence.find(bfs.node->data->GetIdentifier());
            if (pos != lastOccurence.end()) {
                // technology already in contained, update with earlier technology need position
                pos->second++;
            } else {
                // technology is new
                lastOccurence.insert(std::pair<std::string, int>(bfs.node->data->GetIdentifier(), 1));
            }
        }
        ++bfs;
    }

    //std::cout << "passed 1" << std::endl;

    std::map<std::string, int> currentOccurence;
    bfs = tr.begin_breadth_first();
    while(bfs!=tr.end_breadth_first()) {
        //std::cout << bfs.node->data->getGameEntityString() << std::endl;
        if (bfs.node->data->GetTypeOfGameEntity() == TECHNOLOGY) {
            //std::cout << "technology" << std::endl;
            auto pos = currentOccurence.find(bfs.node->data->GetIdentifier());
            if (pos == currentOccurence.end()) {
                //std::cout << "new technology" << std::endl;
                currentOccurence.insert(std::pair<std::string, int>(bfs.node->data->GetIdentifier(), 1));
            } else {
                //std::cout << "repeated technology: increment counter" << std::endl;
                // technology already in contained, update with earlier technology need position
                pos->second++;
                //std::cout << pos->first << " : " << pos->second << std::endl;
            }
            pos = currentOccurence.find(bfs.node->data->GetIdentifier());
            if (pos->second == lastOccurence.at(bfs.node->data->GetIdentifier())) {
                // now we are at last occurence of technology in breath first search -> here we need to build technology tree
                //std::cout << "build subtree and merge" << std::endl;
                // append here to bfs
                tree<GameEntity*> trNew;
                tree<GameEntity*>::iterator rootNew;

                // create tree with new technology as root
                trNew.insert(rootNew, bfs.node->data);
                // get all requirements
                tree<GameEntity*>::breadth_first_iterator newBfs = trNew.begin_breadth_first();
                while(newBfs!=trNew.end_breadth_first()) {
                    GameEntity *nextEntity = newBfs.operator*();
                    std::list<GameEntity *> listOfReq;
                    this->GetRequirementsFromGameEntity(listOfReq, nextEntity, newBfs.node);
                    for (auto req : listOfReq) {
                        // add all requirements as child to current game entity node
                        trNew.append_child(newBfs, req);
                    }
                    // iterate to next node with bfs
                    ++newBfs;
                }

                //std::cout << "before merge" << std::endl;
                //print_GameEntity_Tree(trNew);
                tree<GameEntity*>::iterator it = trNew.begin();
                tr.merge(bfs.begin(), it, false);
                //std::cout << "passed merge" << std::endl;
                //print_GameEntity_Tree(trNew);

            }
        }
        ++bfs;
    }
    return tr;
}

bool Solution::technologyBuildKnown(const std::string& technologyName) {
    return currentTechnologiesTreeMaps.count(technologyName) > 0;
}

void Solution::addEnabledTechnologyBuild(const std::string& technologyName, GameEntity* technology) {
    tree<GameEntity*> trNew;
    tree<GameEntity*>::iterator rootNew;

    // create tree with new technology as root
    trNew.insert(rootNew, technology);
    // get all requirements
    tree<GameEntity*>::breadth_first_iterator newBfs = trNew.begin_breadth_first();
    while(newBfs!=trNew.end_breadth_first()) {
        GameEntity *nextEntity = newBfs.operator*();
        std::list<GameEntity *> listOfReq;
        this->GetRequirementsFromGameEntity(listOfReq, nextEntity, newBfs.node);
        for (auto req : listOfReq) {
            if (req->GetTypeOfGameEntity() == TECHNOLOGY && !technologyBuildKnown(req->GetIdentifier())) {
                std::cout << "was here : " << req->getGameEntityString() << " : " << req->GetIdentifier() << std::endl;
                addEnabledTechnologyBuild(req->GetIdentifier(), req);
                auto it = copyEnabledTechnologyBuilds(req->GetIdentifier());
                trNew.replace(newBfs, it.begin());
            }
            if (req->GetTypeOfGameEntity() == TECHNOLOGY && technologyBuildKnown(req->GetIdentifier())) {
                auto it = copyEnabledTechnologyBuilds(req->GetIdentifier());
                trNew.replace(newBfs, it.begin());
            }
            if (req->GetTypeOfGameEntity() == RECIPE && std::find(this->basisItemsRecipeNames.begin(), this->basisItemsRecipeNames.end(), req->GetIdentifier()) == this->basisItemsRecipeNames.end() ) {
                std::cout << "was here : " << req->getGameEntityString() << " : " << req->GetIdentifier() << std::endl;
                addEnabledRecipeBuild(req->GetIdentifier(), req);
                auto it = copyEnabledRecipeBuilds(req->GetIdentifier(), 0);
                trNew.replace(newBfs, it.begin());
            }
            // add all requirements as child to current game entity node
            trNew.append_child(newBfs, req);
        }
        // iterate to next node with bfs
        ++newBfs;
    }

    currentTechnologiesTreeMaps.insert(std::pair<std::string, tree<GameEntity*>>(technologyName, trNew));
}

tree<GameEntity*> Solution::copyEnabledTechnologyBuilds(const std::string& technologyName) {
    return currentTechnologiesTreeMaps.at(technologyName);
}

bool Solution::recipeBuildKnown(const std::string& recipeName) {
    return currentRecipeTreeMaps.count(recipeName) > 0;
}

void Solution::addEnabledRecipeBuild(const std::string& recipeName, GameEntity* recipe) {
    tree<GameEntity*> trNew;
    tree<GameEntity*>::iterator rootNew;

    // create tree with new technology as root
    trNew.insert(rootNew, recipe);
    // get all requirements
    tree<GameEntity*>::breadth_first_iterator newBfs = trNew.begin_breadth_first();
    while(newBfs!=trNew.end_breadth_first()) {
        GameEntity *nextEntity = newBfs.operator*();
        std::list<GameEntity *> listOfReq;
        this->GetRequirementsFromGameEntity(listOfReq, nextEntity, newBfs.node);
        for (auto req : listOfReq) {
            if (req->GetTypeOfGameEntity() == RECIPE && std::find(this->basisItemsRecipeNames.begin(), this->basisItemsRecipeNames.end(), req->GetIdentifier()) == this->basisItemsRecipeNames.end() ) {
                std::cout << "was here : " << req->getGameEntityString() << " : " << req->GetIdentifier() << std::endl;
                addEnabledRecipeBuild(req->GetIdentifier(), req);
                auto it = copyEnabledRecipeBuilds(req->GetIdentifier(), 0);
                trNew.replace(newBfs, it.begin());
            }
            if (req->GetTypeOfGameEntity() == TECHNOLOGY && technologyBuildKnown(req->GetIdentifier())) {
                auto it = copyEnabledTechnologyBuilds(req->GetIdentifier());
                trNew.replace(newBfs, it.begin());
            }
            if (req->GetTypeOfGameEntity() == TECHNOLOGY && !technologyBuildKnown(req->GetIdentifier())) {
                std::cout << "was here : " << req->getGameEntityString() << " : " << req->GetIdentifier() << std::endl;
                addEnabledTechnologyBuild(req->GetIdentifier(), req);
                auto it = copyEnabledTechnologyBuilds(req->GetIdentifier());
                trNew.replace(newBfs, it.begin());

            }
            // add all requirements as child to current game entity node
            trNew.append_child(newBfs, req);
        }
        // iterate to next node with bfs
        ++newBfs;
    }
    currentRecipeTreeMaps.insert(std::pair<std::string, tree<GameEntity*>>(recipeName, trNew));
}

tree<GameEntity*> Solution::copyEnabledRecipeBuilds(const std::string& recipeName, int amount) {
    //currentRecipeTreeMaps.at(recipeName).node->data->SetNumberOfInstances(amount);
    return currentRecipeTreeMaps.at(recipeName);
}



json Solution::getBuildOrderFromTree(const tree<GameEntity*>& tr) {
    // root is always goal item -> trigger victory event
    // What are leafs??? always player factories
    // Define Building blocks
    // Factory consist of item
    // item consist of recipe
    // recipe consist of items, factory, technology (energy)
    // technology consist of technologies, items (energy)


    // counter to set time stamps of events
    unsigned long long currentTimeStamp = 0;

    // after tree has built without technologies, add technologies in the correct place
    tree<GameEntity*> completeTree = tr;


    std::list<tree_node_<GameEntity*>*> reverseOrder;
    // use BFS
    tree<GameEntity*>::breadth_first_iterator start = tr.begin_breadth_first();
    tree<GameEntity*>::breadth_first_iterator end = tr.end_breadth_first();
    if (!doTechnologyProcessing) {
        while(start != end) {
            //std::cout << (bfs.operator*()->GetIdentifier()) << ", " << (bfs.operator*()->getGameEntityString()) << std::endl << std::flush;
            // add in front to list for reverse order
            // Therefore use nodes to keep informations where it is in tree
            reverseOrder.push_front(start.node);
            // iterate through BFS
            ++start;
        }
        doTechnologyProcessing = true;
        completeTree = addTechnologyProcessingToTree(tr, reverseOrder);
    }


    // now we need full reverse order
    // use BFS
    start = completeTree.begin_breadth_first();
    end = completeTree.end_breadth_first();

    //std::cout << "start bfs" << std::endl;
    std::list<tree_node_<GameEntity*>*> completeReverseOrder;
    while(start != end) {
        //std::cout << (bfs.operator*()->GetIdentifier()) << ", " << (bfs.operator*()->getGameEntityString()) << std::endl << std::flush;
        // add in front to list for reverse order
        // Therefore use nodes to keep informations where it is in tree
        completeReverseOrder.push_front(start.node);
        // iterate through BFS
        ++start;
    }

    // Create all events by processing each node of the reverse BFS list
    // We processing node by node therefore, we have a sequential order of events and nothing is done in parallel
    std::list<json> allEventsList;
    std::list<std::string> enabledTechnology;
    std::list<std::string> processedRecipes;

    for (auto node : completeReverseOrder) {
        //std::cout << "reverse Order: " << node->data->GetIdentifier() << std::endl;
        auto typeOfGameEntity = node->data->GetTypeOfGameEntity();

        // processing of each GameEntity (start-factory-event, stop-factory-event, build-factory-event, technology-event)
        if (typeOfGameEntity == RECIPE) {
            // ##### processing of RECIPE events
            // RECIPE children:
            // processing FACTORY(player, else), TECHNOLOGY, multiple ITEMs (ingredients)
            if (true) {
                if (std::find(processedRecipes.begin(), processedRecipes.end(), node->data->GetIdentifier()) == processedRecipes.end()) {
                    // Recipe we want to process
                    auto recipeToProcess = node;
                    // get factory which should process recipe
                    auto responsibleFactory = node->first_child;
                    // RECIPE has always a FACTORY child!
                    do {
                        if (responsibleFactory->data->GetTypeOfGameEntity() == FACTORY) {
                            break;
                        }
                    } while (responsibleFactory != node->last_child);

                    if (responsibleFactory->data->GetIdentifier() != "player") {
                        // Factory that is going to be build
                        GameEntity *factoryToBuild = responsibleFactory->data;
                        // item containing the factory (not needed)
                        GameEntity *itemFactory = responsibleFactory->first_child->data;
                        json buildFactoryEvent;
                        buildFactoryEvent["type"] = "build-factory-event";
                        buildFactoryEvent["timestamp"] = currentTimeStamp;
                        buildFactoryEvent["factory-type"] = factoryToBuild->GetIdentifier();
                        buildFactoryEvent["factory-name"] = itemFactory->GetIdentifier(); // only for debugging
                        buildFactoryEvent["factory-id"] = factoryToBuild->getFactoryID();
                        allEventsList.push_front(buildFactoryEvent);
                    }

                    //std::cout << "RECIPE: " << node->data->GetIdentifier() << " will be produced by: " << responsibleFactory->data->GetIdentifier() << std::endl;
                    // create events for start and stop factory

                    // find all other nodes with same recipe type
                    int sumOfRecipeProcessings = 0;
                    for (auto possibleRecipes : completeReverseOrder) {
                        if (possibleRecipes->data->GetTypeOfGameEntity() == RECIPE && possibleRecipes->data->GetIdentifier() == node->data->GetIdentifier()) {
                            sumOfRecipeProcessings += possibleRecipes->data->GetNumberOfInstances();
                            //std::cout << recipeToProcess->data->GetIdentifier() << " : " << sumOfRecipeProcessings << std::endl;
                        }
                    }
                    recipeToProcess->data->SetNumberOfInstances(sumOfRecipeProcessings);
                    // << recipeToProcess->data->GetIdentifier() << " : "
                    //          << recipeToProcess->data->GetNumberOfInstances() << std::endl;
                    processedRecipes.push_front(node->data->GetIdentifier());

                    std::tuple<json, json, unsigned long long> tuple = createFactoryEventsForRecipe(currentTimeStamp,
                                                                                                    *responsibleFactory,
                                                                                                    *recipeToProcess);
                    json startFactoryEvent = std::get<0>(tuple);
                    json stopFactoryEvent = std::get<1>(tuple);
                    currentTimeStamp = std::get<2>(tuple);
                    allEventsList.push_front(startFactoryEvent);
                    allEventsList.push_front(stopFactoryEvent);
                    }
                }
        } else if (typeOfGameEntity == TECHNOLOGY) {
            // Technology to be researched
            GameEntity* technology = node->data;
            std::string technologyName = technology->GetIdentifier();
            if (std::find(enabledTechnology.begin(), enabledTechnology.end(), technologyName) != enabledTechnology.end()) {
                // technology already enabled do nothing
            } else {
                enabledTechnology.push_front(technologyName);
                currentTimeStamp = currentTimeStamp;
                json researchEvent;
                researchEvent["type"] = "research-event";
                researchEvent["technology"] = technology->GetIdentifier();
                researchEvent["timestamp"] = ++currentTimeStamp;
                allEventsList.push_front(researchEvent);
            }
        }
    }

    json victoryEvent;
    victoryEvent["type"] = "victory-event";
    victoryEvent["timestamp"] = currentTimeStamp;

    json allEvents = {victoryEvent};
    for (const json& eventJson : allEventsList) {
        allEvents.push_back(eventJson);
    }
    //std::cout << std::setw(4) << allEvents << std::endl;
    return allEvents;
}


json Solution::getSolJson() {
    tree<GameEntity*> gameEntityTree = buildGoalItemGameEntityTree();
    json events = getBuildOrderFromTree(gameEntityTree);

    return events;
}

// challenge 2 Requirements:
// build-factory-events
// factory
//      item(factory)
//          recipe(factory)
//              factory("player") + ingredients + (no technologies)
// prevent loops

// challenge 4 Requirements:
// add research

// Ideas for faster computation:
// 1. do same research only once
//      keep them in list and only create technology requirement when not already available
//      for event creation we have to start also from the right side and iterate such that research is done before needed

// 2. group mining events to have smaller out print: difficult
//      start with bfs
//      take current recipe node and find all other same recipe nodes (they can be processed together as they are already enabled)
//      compute time for all recipes and return time