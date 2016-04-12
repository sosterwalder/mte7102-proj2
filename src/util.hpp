#pragma once

#include "main.hpp"

class Util
{
public:
    static bool fileExists(const std::string &filename);
    static const std::string fileToString(const std::string &filename);
    static std::vector<std::pair<std::string, std::string>> findShaderObjects(const std::string &path);
};
