//
// Created by johan on 12.01.2021.
//

#include "../headers/Technology.hpp"

//TODO consider moving parameters
Technology::Technology(std::string _name, std::vector<std::string> _prerequisites, std::vector<Recipe *> _recipes,
                       std::map<std::string, int> _ingredients) : name(_name), prerequisites(_prerequisites),
                                                                  recipes(_recipes),
                                                                  ingredients(_ingredients) {
}

bool Technology::checkPrerequisites(const std::vector<std::string> &technologies) {
    return prerequisites == technologies;
}

void pushDiffBack(std::map<std::string, int> &itemsAvailable, std::map<std::string, int> &diff){
    for(const auto& d: diff){
        itemsAvailable.at(d.first) = d.second;
    }
}

bool Technology::execute(std::map<std::string, int> &itemsAvailable, std::vector<Technology> &curTechnologies) {
    std::map<std::string, int> diff;
    std::vector<std::string> techVec;
    //reserves the memory space for the vector (improves run time for following for loop)
    techVec.reserve(curTechnologies.size());
    for (auto t : curTechnologies) {
        techVec.push_back(t.getName());
    }
    if (!checkPrerequisites(techVec)) return false;

    for(auto &ing : ingredients){
        try {
            int &amount = itemsAvailable.at(ing.first);
            if (amount >= ing.second){
                int test = amount;
                diff.insert(std::pair<std::string, int>(ing.first, test));//TODO test if this really copies the amount instead of the reference
                amount -= ing.second;
            }
            else {
                pushDiffBack(itemsAvailable, diff);
                return false;
            }
        } catch (std::out_of_range &e){
            pushDiffBack(itemsAvailable, diff);
            return false;
        }
    }
    curTechnologies.push_back(*this);
    return true;
}

bool Technology::operator==(const Technology &tech) const {
    return name == tech.name;
}

bool Technology::operator<(const Technology &tech) const {
    int c = name.compare(tech.name);
    if (c < 0) return true;
    else return false;
}

bool Technology::operator>(const Technology &tech) const {
    int c = name.compare(tech.name);
    if (c > 0) return true;
    else return false;
}

std::string Technology::toString() {
    std::string t;
    t += name;
    t += "\n\tprerequisites: ";
    t += "\n\t";
    for (const auto &s: prerequisites) {
        t += s + ",";
    }
    return t;
}

std::vector<Recipe *> Technology::getRecipes() {
    return recipes;
}

std::string Technology::getName() {
    return name;
}

Technology::~Technology() {
    for (auto r : recipes) {
        delete r;
    }
}
