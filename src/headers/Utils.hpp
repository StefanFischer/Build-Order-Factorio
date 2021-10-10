//
// Created by johannes on 18.01.21.
//
#include <fstream>
#include <map>

#ifndef ADVPT_PROJECT_UTILS_HPP
#define ADVPT_PROJECT_UTILS_HPP


extern std::string g_dataPath;
extern std::string g_inputPath;
extern std::string g_buildOrdersPath;

extern std::string g_settingsJsonPath;
extern std::string g_factoryJsonPath;
extern std::string g_itemJsonPath;
extern std::string g_recipeJsonPath;
extern std::string g_techJsonPath;

extern std::string g_challenge1JsonPath;
extern std::string g_challenge2JsonPath;
extern std::string g_challenge3JsonPath;
extern std::string g_challenge4JsonPath;
extern std::string g_challenge5JsonPath;
extern std::string g_challenge6JsonPath;
extern std::string g_challenge7JsonPath;

class Utils {
public:
    static std::string map_to_string(const std::map<std::string, int> &m);
};

#endif //ADVPT_PROJECT_UTILS_HPP
