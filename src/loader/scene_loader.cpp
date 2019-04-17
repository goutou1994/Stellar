#include <fstream>
#include <sstream>
#include <vector>
#include "scene_loader_functions.h"
#include "Exceptions.h"
#include <iostream>

using namespace std;

Scene* loadSceneFromFile(std::string path, Scene *scene) {
    ifstream file(path);
    string line;
    if (scene == nullptr) scene = new Scene();
    if (!file.is_open()) {
        std::cerr << "Error opening file " << path << std::endl;
    }
    while (!file.eof()) {
        getline(file, line);
        istringstream ss(line);
        vector<string> words;
        while (!ss.eof()) {
            words.emplace_back();
            ss >> *words.rbegin();
        }
        try {
            scene_loader_functions::functions.at({words[0], words.size()})(*scene, words);
        } catch (out_of_range &e) {
            std::cerr << "unknown command: " << line << std::endl;
        } catch (command_error &e) {
            std::cerr << "command error (" << e.what() << "): " << line << std::endl;
        }

    }
    file.close();
    return scene;
}