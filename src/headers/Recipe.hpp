//
// Created by stefa on 06.01.2021.
//
#include <fstream>
#include <map>

#ifndef ADVPT_PROJECT_RECIPE_HPP
#define ADVPT_PROJECT_RECIPE_HPP

class Recipe {
private:
    std::string name;
    std::string category;
    int energy;
    //items , amount
    std::map<std::string, int> ingredients;
    //items, amount
    std::map<std::string, int> products;
    // recipe currently enabled
    bool enabled = false;
public:
    Recipe(std::string name, std::string category, int energy, std::map<std::string, int> ingredients,
           std::map<std::string, int> products, bool enabled);

    std::string getName();

    /**
    * @return a map of the item names and their respective amount
    */
    const std::map<std::string, int>& getIngredients();

    int getEnergy() const;

    /**
     * executes the recipe. Alters the given map to the new state.
     */
    void execute(std::map<std::string, int> &itemsAvailable);

    /**
     * @return a map of the item names and their respective amount
     */
    std::map<std::string, int> getProducts();

    std::string getCategory();

    bool isEnabled() const;
};


#endif //ADVPT_PROJECT_RECIPE_HPP
