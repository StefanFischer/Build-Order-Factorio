//
// Created by johannes on 18.01.21.
//

#include "../headers/Utils.hpp"

std::string g_dataPath = "../jsonFiles/factorio-data/";
std::string g_inputPath = "../jsonFiles/factorio-simulator/inputs/";
std::string g_buildOrdersPath = "../jsonFiles/factorio-simulator/build-orders/";

std::string g_settingsJsonPath = g_dataPath + "default-settings.json";
std::string g_factoryJsonPath = g_dataPath + "factory.json";
std::string g_itemJsonPath = g_dataPath + "item.json";
std::string g_recipeJsonPath = g_dataPath + "recipe.json";
std::string g_techJsonPath = g_dataPath + "technology.json";

std::string g_challenge1JsonPath = g_inputPath + "challenge-1.json";
std::string g_challenge2JsonPath = g_inputPath + "challenge-2.json";
std::string g_challenge3JsonPath = g_inputPath + "challenge-3.json";
std::string g_challenge4JsonPath = g_inputPath + "challenge-4.json";
std::string g_challenge5JsonPath = g_inputPath + "challenge-5.json";
std::string g_challenge6JsonPath = g_inputPath + "challenge-6.json";
std::string g_challenge7JsonPath = g_inputPath + "challenge-7.json";

std::string Utils::map_to_string(const std::map<std::string, int> &m) {
    std::string output;
    std::string convrt;
    std::string result;

    for (const auto &it : m) {
        convrt = std::to_string(it.second);
        output += (it.first) + ":" + (convrt) + ", ";
    }

    result = output.substr(0, output.size() - 2);

    return result;

}
