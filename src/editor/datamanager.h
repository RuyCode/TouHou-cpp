#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "game.pb.h"

class DataManager
{
public:
    DataManager() = delete;
    ~DataManager();
    DataManager(const DataManager &) = delete;
    DataManager(DataManager &&) = delete;
    DataManager &operator=(const DataManager &) = delete;
    DataManager &operator=(DataManager &&) = delete;

    static void loadLevel(const std::string &file_path);
    static void saveLevel();

    static const std::string &getFilePath();
    static game::Level &getLevel();

private:
    static std::string m_file_path;

    static game::Level m_level;
};

#endif
