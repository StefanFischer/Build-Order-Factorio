#ifndef ADVPT_PROJECT_GAMEENTITY_H
#define ADVPT_PROJECT_GAMEENTITY_H

#include<string>
#include <list>
#include "Recipe.hpp"
#include "tree.hh"

enum TypeOfGameEntity {ITEM, FACTORY, RECIPE, TECHNOLOGY};

/**
 * This class belongs to the computation of a valid build tree.
 * 
 * A GameEntity object can represent an item, a factory, a recipe
 * or a technology. It is used as the node type of the tree.
 */
class GameEntity 
{
    TypeOfGameEntity typeOfGameEntity;

    // A string that identifies the specific variant of the entity
    // (e.g. the type/name of the factory)
    std::string identifier;

    // There might be more than one instance of this entity
    // Used for Number of Items and Number of Recipe processing to achieve needed amount of Items
    int numberOfInstances;

    // Keep Factory ID also in Game Entity
    // only set for FACTORY game entity, else -1
    // Factory "player" has factory ID=0
    // set this is GameEntity of Type FACTORY is created
    int factoryID;
    // Counter for Factories: incremented if new FACTORY-GameEntity is created
    static int factoryCounter;

    // Save location in tree for backtracking to identify loops
    tree_node_<GameEntity*>* treeNode;

    // Save alternative RECIPES for backtracking to fix loops
    std::list<Recipe*> alternativeRecipes;

public:
    // Constructor: with empty RECIPE-alternative list
    GameEntity(TypeOfGameEntity type, const std::string& identifier, int numberOfInstances, tree_node_<GameEntity*>* treeNode);
    // Constructor: with RECIPE-alternative list
    GameEntity(TypeOfGameEntity type, const std::string& identifier, int numberOfInstances, tree_node_<GameEntity*>* treeNode, std::list<Recipe*> alternativeRecipes);

    // Destructor
    ~GameEntity();

    // Getters
    TypeOfGameEntity GetTypeOfGameEntity();
    const std::string& GetIdentifier();
    int GetNumberOfInstances();
    int getFactoryID();
    std::list<Recipe*> getAlternativeRecipes();

    // Setters
    void SetTypeOfGameEntity(TypeOfGameEntity typeOfGameEntity);
    void SetIdentifier(const std::string& identifier);
    void SetNumberOfInstances(int numberOfInstances);

    // print method
    std::string getGameEntityString();
};

#endif // ADVPT_PROJECT_GAMEENTITY_H
