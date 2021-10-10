//
// Created by johan on 12.01.2021.
//

#include "../headers/Recipe.hpp"

//TODO consider moving (std::move()) parameters instead of copying them
Recipe::Recipe(std::string name, std::string category, int energy, std::map<std::string, int> ingredients,
               std::map<std::string, int> products, bool enabled) {
    this->name = name;
    this->category = category;
    this->products = products;
    this->energy = energy;
    this->ingredients = ingredients;
    this->enabled = enabled;
}

std::string Recipe::getName() {
    return this->name;
}

void Recipe::execute(std::map<std::string, int> &itemsAvailable) {
    for(auto p : products){
	auto s = itemsAvailable.find(p.first);
        if(s != itemsAvailable.end()){//itemsAvailable.contains(p.first)){ uni cip has no c++20 version
            itemsAvailable.at(p.first) += p.second;
        }else{
            itemsAvailable.insert(p);
        }
    }
}

std::map<std::string, int> Recipe::getProducts() {
    return products;
}

const std::map<std::string, int> & Recipe::getIngredients() {
    return ingredients;
}

std::string Recipe::getCategory() {
    return category;
}

int Recipe::getEnergy() const {
    return energy;
}

bool Recipe::isEnabled() const {
    return this->enabled;
}
