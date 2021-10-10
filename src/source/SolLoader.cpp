//
// Created by johan on 28.01.2021.
//

#include "../headers/SolLoader.hpp"
#include "../headers/Solution.hpp"

#include <memory>
#include <sstream>
#include <regex>

std::string decodeHtml(const std::string & html) {
    // {&quot;initial-items&quot;: [], &quot;initial-factories&quot;:
    std::regex long_word_regex("&quot;");
    return std::regex_replace(html, long_word_regex, "\"");
}

void SolLoader::init(std::string &challengePath) { // -t "../target.json"
    // initialize simulator as Ex 3.1

    // ######## Step 1: load challenge initial settings ########
    // load challenge json file
    //std::cout << challengePath;
    //std::cout << "Hello again" << std::endl;

    std::ifstream challengeStream(challengePath);
    if (!challengeStream.good()) {
        std::cout << "ERROR for " << challengePath << "  : " << std::strerror(errno) << std::endl;
        exit(-1);
    }


    std::string line;
    std::string s;
    while (!challengeStream.eof()) {
        std::getline(challengeStream, line);
        s += decodeHtml(line);
    }
    // read json file
    json challengeJson = json::parse(s);
    challengeStream.close();
    //std::cout << "and again" << std::endl;
    //std::cout << challengeJson << std::endl;

    //load all json files and parse them
    std::ifstream factoryStream(g_factoryJsonPath);
    factoryJson = json::parse(factoryStream);
    factoryStream.close();

    std::ifstream itemStream(g_itemJsonPath);
    itemJson = json::parse(itemStream);
    itemStream.close();

    std::ifstream recipeStream(g_recipeJsonPath);
    recipeJson = json::parse(recipeStream);
    recipeStream.close();

    std::ifstream techStream(g_techJsonPath);
    technologyJson = json::parse(techStream);
    techStream.close();

    // fill vectors and maps of game state
    for (json::iterator it = challengeJson.begin(); it != challengeJson.end(); ++it) {
        //std::cout << it.key() << " : " << it.value() << "\n";
        auto list = it.value();
        for (json::iterator it2 = list.begin(); it2 != list.end(); ++it2) {
            auto tuple = it2.value();
            //std::cout << it.key() << " : " << tuple << "\n";
            if (it.key() == "initial-items") {
                std::string name = tuple.at("name");
                int amount = tuple.at("amount");
                //item *newItem = new item(name);
                this->currentItems.insert(std::pair<std::string, int>(name, amount));
                //std::cout << "name: " << name << " , amount: " << amount << "\n";
            } else if (it.key() == "initial-factories") {
                std::string factoryName = tuple.at("factory-name");
                int factoryId = tuple.at("factory-id");
                std::string factoryType = tuple.at("factory-type");
                //std::cout << "Fac type: " << factoryType << std::endl;
                auto *newFactory = new Factory(factoryName, factoryType, factoryId);
                this->factories.push_back(newFactory);
                //std::cout << "Factory-name: " << factoryName << " , Factory-Id: " << factoryId << " , Factory-Type: " << factoryType << "\n";
            } else if (it.key() == "goal-items") {
                std::string name = tuple.at("name");
                int amount = tuple.at("amount");
                //item *newItem = new item(name);
                this->goalItems.insert(std::pair<std::string, int>(name, amount));
                //std::cout << "goal item name: " << name << " , amount: " << amount << "\n";
            }
        }
    }

    // get all recipes which are enabled at start
    for (json::iterator it = recipeJson.begin(); it != recipeJson.end(); ++it) {
        const auto &recipeName = it.key();
        auto list = it.value();
        auto enabled = list["enabled"];
        //if (enabled) {
        auto category = list["category"];
        auto energy = list["energy"];
        auto ingredients = list["ingredients"];
        auto products = list["products"];
        std::map<std::string, int> ingredientsMap;
        std::map<std::string, int> productsMap;
        for (json::iterator it2 = ingredients.begin(); it2 != ingredients.end(); ++it2) {
            auto tuple = it2.value();
            std::string name = tuple["name"];
            int amount = tuple["amount"];
            //std::cout << "ingredients: " << name << " amount: " << amount << std::endl;
            ingredientsMap.insert(std::pair<std::string, int>(name, amount));
        }
        for (json::iterator it2 = products.begin(); it2 != products.end(); ++it2) {
            auto tuple = it2.value();
            std::string name = tuple["name"];
            int amount = tuple["amount"];
            //std::cout << "products: " << name << " amount: " << amount << std::endl;
            productsMap.insert(std::pair<std::string, int>(name, amount));
        }
        //std::cout << "enabled: " << enabled << std::endl;
        auto *recipe = new Recipe(recipeName, category, energy, ingredientsMap, productsMap, enabled);
        this->allRecipes.push_back(recipe);
        if (enabled) {
            this->currentRecipes.push_back(recipe);
        }
    }
}

Factory *SolLoader::getFactoryFromJson(const std::string &factoryName) {
    Factory *factory = new Factory(factoryName, factoryName);
    return factory;
}

Technology *SolLoader::getTechnologyFromJson(const std::string &techName) {
    json tech = technologyJson[techName];
    json effects = tech["effects"];
    json ingredientsJson = tech["ingredients"];
    auto prerequisites = tech["prerequisites"].get<std::vector<std::string>>();
    std::vector<Recipe *> recipes;
    for (auto eff : effects) {
        if (eff["type"] != "unlock-recipe") throw std::invalid_argument("technology has unexpected effect!");
        std::string rName = eff["recipe"];
        Recipe *r = getRecipeFromJson(rName);
        recipes.push_back(r);
    }

    auto ingredients = getIngredients(ingredientsJson);
    return new Technology(techName, prerequisites, recipes, ingredients);
}

std::map<std::string, int> SolLoader::getIngredients(json &ingredientsJson) {
    std::map<std::string, int> ingredients;
    for (json ing : ingredientsJson) {
        ingredients.insert(std::pair<std::string, int>(ing["name"], ing["amount"]));
    }
    return ingredients;
}

Recipe *SolLoader::getRecipeFromJson(const std::string &recipeName) {
    json recipe = recipeJson[recipeName];
    std::string category = recipe["category"];
    bool enabled = recipe["enabled"];
    int energy = recipe["energy"];
    json ingredientsJson = recipe["ingredients"];
    auto ingredients = getIngredients(ingredientsJson);

    json productsJson = recipe["products"];
    auto products = getIngredients(productsJson);

    return new Recipe(recipeName, category, energy, ingredients, products, enabled);
}

std::unique_ptr<Solution> SolLoader::createSolution(std::string &challengePath) {

    init(challengePath);
    return std::make_unique<Solution>(settingsJson, factoryJson, itemJson, recipeJson, technologyJson,
                                      currentItems, currentRecipes, allRecipes,
                                      currentTechnologies, factories, goalItems, processingEvents);
}

