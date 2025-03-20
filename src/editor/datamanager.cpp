#include "datamanager.h"

#include <fstream>

DataManager& DataManager::GetInstance() {
    static DataManager instance;
    return instance;
}

void DataManager::LoadLevel(const std::string& file_path) {
    level.Clear();
    std::ifstream in(file_path);
    level.ParseFromIstream(&in);
    in.close();
}

DataManager::DataManager() {
}

DataManager::~DataManager() {
    level.Clear();
}
