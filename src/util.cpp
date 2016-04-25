#include "util.hpp"


NAMESPACE_BEGIN(QCE);

bool Util::fileExists(const std::string &fileName)
{
    return access(fileName.c_str(), 0) == 0;
}

const std::string Util::fileToString(const std::string &filename)
{
    if (filename.empty()) {
        return "";
    }

    std::ifstream t(filename);

    return std::string(
        (std::istreambuf_iterator<char>(t)),
        std::istreambuf_iterator<char>()
    );
}



std::vector<std::pair<std::string, std::string>> Util::findShaderFiles(const std::string &path)
{
    std::vector<std::pair<std::string, std::string>> result;
    DIR *dp = opendir(path.c_str());

    if (!dp) {
        std::string errorMessage = fmt::format("Could not open shader object directory at '{}'", path);
        spdlog::get("qde")->error(errorMessage);
        throw std::runtime_error(errorMessage);
    }

    struct dirent *ep;

    while ((ep = readdir(dp))) {
        const char *fname = ep->d_name;

        if (strstr(fname, "xml") == nullptr) {
            continue;
        }

        std::string fileName = std::string(fname);
        std::string fullName = path + "/" + fileName;

        if (!Util::fileExists(fullName)) {
            std::string errorMessage = fmt::format("Found shader object '{}' is not readable", fullName);
            spdlog::get("qde")->error(errorMessage);
            throw std::runtime_error(errorMessage);
        }
        result.push_back(std::make_pair(
            fileName.substr(0, std::string(fileName).length() - 7),
            fullName
        ));
    }

    closedir(dp);

    return result;
}

NAMESPACE_END(QCE);
