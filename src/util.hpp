#pragma once

// System imports
#include <unistd.h>
#include <sys/dir.h>

// Project imports
#include "common.hpp"


NAMESPACE_BEGIN(QCE);

class Util
{
public:
    static bool fileExists(const std::string &filename);
    static const std::string fileToString(const std::string &filename);
    static std::vector<std::pair<std::string, std::string>> findShaderFiles(const std::string &path);
};

NAMESPACE_END(QCE);
