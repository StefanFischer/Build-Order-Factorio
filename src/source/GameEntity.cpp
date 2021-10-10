#include "../headers/GameEntity.hpp"


int GameEntity::factoryCounter = 1;

// Constructor
GameEntity::GameEntity(TypeOfGameEntity type, const std::string& identifier, int numberOfInstances, tree_node_<GameEntity*>* treeNode, std::list<Recipe*> alternativeRecipes)
    : typeOfGameEntity(type), identifier(std::move(identifier)), numberOfInstances(numberOfInstances), treeNode(treeNode), alternativeRecipes(alternativeRecipes)
{
    // set factory ID
    if (type == FACTORY && identifier != "player") {
        this->factoryID = factoryCounter;
        ++factoryCounter;
    } else if (type == FACTORY && identifier == "player") {
        this->factoryID = 0;
    } else {
        factoryID = -1;
    }
}

GameEntity::GameEntity(TypeOfGameEntity type, const std::string& identifier, int numberOfInstances, tree_node_<GameEntity*>* treeNode)
        : typeOfGameEntity(type), identifier(std::move(identifier)), numberOfInstances(numberOfInstances), treeNode(treeNode)
{
    // set factory ID
    if (type == FACTORY && identifier != "player") {
        this->factoryID = factoryCounter;
        ++factoryCounter;
    } else if (type == FACTORY && identifier == "player") {
        this->factoryID = 0;
    } else {
        factoryID = -1;
    }

    // init empty alternative RECIPES list
    std::list<Recipe*> emptyList;
    this->alternativeRecipes = emptyList;
}

// Destructor
GameEntity::~GameEntity()
{}

// Getters
TypeOfGameEntity GameEntity::GetTypeOfGameEntity()
{
    return this->typeOfGameEntity;
}

const std::string& GameEntity::GetIdentifier()
{
    return this->identifier;
}

int GameEntity::GetNumberOfInstances()
{
    return this->numberOfInstances;
}


// Setters
void GameEntity::SetTypeOfGameEntity(TypeOfGameEntity type)
{
    this->typeOfGameEntity = type;
}
    
void GameEntity::SetIdentifier(const std::string& id)
{
    this->identifier = std::move(id);
}

void GameEntity::SetNumberOfInstances(int num)
{
    this->numberOfInstances = num;
}

std::string GameEntity::getGameEntityString() {
    std::string s;
    switch (this->GetTypeOfGameEntity()) {
        case ITEM:
            s = "ITEM";
            break;
        case FACTORY:
            s = "FACTORY";
            break;
        case RECIPE:
            s = "RECIPE";
            break;
        case TECHNOLOGY:
            s = "TECHNOLOGY";
            break;
    }
    return s;
}

int GameEntity::getFactoryID() {
    return this->factoryID;
}

std::list<Recipe*> GameEntity::getAlternativeRecipes() {
    return this->alternativeRecipes;
}