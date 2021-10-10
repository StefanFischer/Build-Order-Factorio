#include <iostream>
#include <iomanip>
#include <chrono>
#include "headers/SolLoader.hpp"
#include "headers/Utils.hpp"

using json = nlohmann::json;

std::string usage = "Usage: \n\t-t target.json\n\t[optional] -s settings.json";


int main(int argc, char *argv[]) {

    std::string settingsPath;
    std::string inputPath;

    // TODO: set to true if production
    if (false) {
        std::ifstream f(inputPath);
        std::cout << f.rdbuf() << std::endl;
        switch (argc) {
            case 3:
                if (std::string(argv[1]) == "-t") {
                    //inputPath = g_inputPath + std::string(argv[2]);
                    inputPath = std::string(argv[2]);
                    break;
                } else
                    std::cout << usage << std::endl;
                return -1;
            case 5:
                if (std::string(argv[1]) == "-t" && std::string(argv[3]) == "-s") {
                    inputPath = std::string(argv[2]);
                    settingsPath = std::string(argv[4]);
                    break;
                } else if (std::string(argv[3]) == "-t" && std::string(argv[1]) == "-s") {
                    inputPath = std::string(argv[4]);
                    settingsPath = std::string(argv[2]);
                    break;
                }
                //no break goes to next case and returns error
            default:
                std::cout << usage << std::endl;
                return -1;
        }
    } else {
        // TODO: for testing
        //inputPath = g_inputPath + "sulfuric-acid.json";
        inputPath = g_inputPath + "challenge-7.json";
    }

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    SolLoader loader;
    auto sol = loader.createSolution(inputPath);
    json solJson = sol->getSolJson();
    //std::cout << std::setw(4) << solJson << std::endl;
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::ofstream out("buildOrder.json");
    out <<std::setw(4)<< solJson << std::endl;
    out.close();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;

}
