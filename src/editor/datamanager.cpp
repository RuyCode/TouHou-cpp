#include "datamanager.h"

#include <fstream>

game::Level DataManager::level = game::Level();
std::string DataManager::file_path = "";

DataManager& DataManager::GetInstance() {
    static DataManager instance;
    return instance;
}

void DataManager::LoadLevel(const std::string& file_path) {
    DataManager::file_path = file_path;

    level.Clear();
    std::ifstream in(file_path);
    level.ParseFromIstream(&in);
    in.close();
}

void DataManager::SaveLevel() {
    if (file_path.empty()) {
        return;
    }

    std::ofstream out(file_path, std::ios::trunc | std::ios::out);
    level.SerializeToOstream(&out);
    out.close();
}

DataManager::DataManager() {
}

DataManager::~DataManager() {
    level.Clear();
}
