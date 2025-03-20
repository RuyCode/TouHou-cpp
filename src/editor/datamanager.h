#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "game.pb.h"

class DataManager {
public:
    static DataManager& GetInstance();

    void LoadLevel(const std::string& file_path);

    game::Level level;

private:
    DataManager();
    ~DataManager();
    DataManager(const DataManager&) = delete;
    DataManager(DataManager&&) = delete;
    DataManager& operator=(const DataManager&) = delete;
    DataManager& operator=(DataManager&&) = delete;
};

#endif
