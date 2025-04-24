#include "datamanager.h"

#include <fstream>

game::Level DataManager::m_level = game::Level();
std::string DataManager::m_file_path = "";

void DataManager::loadLevel(const std::string &file_path)
{
    DataManager::m_file_path = file_path;

    m_level.Clear();
    std::ifstream in(file_path);
    m_level.ParseFromIstream(&in);
    in.close();
}

void DataManager::saveLevel()
{
    if (m_file_path.empty()) {
        return;
    }

    std::ofstream out(m_file_path, std::ios::trunc | std::ios::out);
    m_level.SerializeToOstream(&out);
    out.close();
}

const std::string &DataManager::getFilePath()
{
    return m_file_path;
}

game::Level &DataManager::getLevel()
{
    return m_level;
}

DataManager::~DataManager()
{
    m_level.Clear();
}
