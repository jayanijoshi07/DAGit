#include <iostream>
#include <filesystem>
#include <fstream>

#include "../../include/core/repository.hpp"

namespace fs = std::filesystem;

namespace core
{

bool Repository::init(const std::string& path)
{
    try
    {
        fs::path repoPath = fs::path(path) / ".dagit";

        if(fs::exists(repoPath))
        {
            std::cout << ".dagit already exists" << std::endl;
            return false;
        }

        fs::create_directory(repoPath);

        fs::create_directory(repoPath / "objects");

        fs::create_directories(repoPath / "refs" / "heads");

        std::ofstream index_file(repoPath / "index");

        if(!index_file)
        {
            std::cerr << "Failed to create index" << std::endl;
            return false;
        }

        std::ofstream head_file(repoPath / "HEAD");

        if(!head_file)
        {
            std::cerr << "Failed to create HEAD" << std::endl;
            return false;
        }

        head_file << "main\n";

        std::cout << "Initialized empty DAGit repository in " << repoPath << std::endl;

        return true;
    }
    catch(const fs::filesystem_error& e)
    {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        return false;
    }
}

std::optional<fs::path> Repository::find_repo_root(const fs::path& start)
{
    fs::path current = start;
    fs::path prev = "";

    while(prev != current)
    {
        if(fs::exists(current / ".dagit"))
        {
            return current;
        }

        prev = current;
        current = current.parent_path();
    }

    return std::nullopt;
}

}