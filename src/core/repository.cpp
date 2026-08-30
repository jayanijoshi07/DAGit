#include <iostream>
#include <filesystem>

#include "../../include/core/repository.hpp"

namespace fs = std::filesystem;

namespace core
{

bool Repository::init(const std::string& path)
{
    try
    {
        fs::path repoPath =
            fs::path(path) / ".dagit";

        // Check if already exists
        if(fs::exists(repoPath))
        {
            std::cout
                << ".dagit already exists"
                << std::endl;

            return false;
        }

        // Create .dagit
        fs::create_directory(repoPath);

        // Create .dagit/objects
        fs::create_directory(
            repoPath / "objects"
        );
                               
        std::cout
            << "Initialized empty DAGit repository in "
            << repoPath
            << std::endl;

        return true;
    }
    catch(const fs::filesystem_error& e)
    {
        std::cerr
            << "Filesystem error: "
            << e.what()
            << std::endl;

        return false;
    }
} // bool Repository::init(const std::string& path)

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
} // std::optional<fs::path> Repository::find_repo_root(const fs::path& start)

} // namespace core {