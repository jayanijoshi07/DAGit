#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <stdexcept>
#include <set>

#include "../../include/commands/status_command.hpp"
#include "../../include/core/repository.hpp"
#include "../../include/core/object_store.hpp"

void StatusCommand::setup(CLI::App& app)
{
    auto cmd = app.add_subcommand("status", "Show the working tree status");

    cmd->callback([&]()
    {
        execute();
    });
}

void StatusCommand::execute()
{
    auto repo = core::Repository::find_repo_root();

    if (!repo)
    {
        throw std::runtime_error("Not inside DAGit repository");
    }

    std::filesystem::path index_path = *repo / ".dagit" / "index";

    std::ifstream index(index_path);

    if (!index)
    {
        throw std::runtime_error("Failed to open index");
    }

    std::set<std::filesystem::path> tracked_files;
    bool has_changes = false;

    std::string line;

    while (std::getline(index, line))
    {
        std::stringstream ss(line);

        std::string file_path;
        std::string staged_oid;

        ss >> file_path >> staged_oid;

        std::filesystem::path file = *repo / file_path;

        tracked_files.insert(file);

        if (!std::filesystem::exists(file))
        {
            std::cout << "deleted: " << file_path << std::endl;
            has_changes = true;
            continue;
        }

        std::ifstream in(file, std::ios::binary);

        if (!in)
        {
            throw std::runtime_error(
                "Failed to open file: " + file_path
            );
        }

        std::stringstream buffer;
        buffer << in.rdbuf();

        std::string content = buffer.str();

        std::string current_oid =
            core::ObjectStore::hash_object(content, "blob");

        if (current_oid != staged_oid)
        {
            std::cout << "modified: " << file_path << std::endl;
            has_changes = true;
        }
    }

    for (const auto& entry :
         std::filesystem::recursive_directory_iterator(*repo))
    {
        if (entry.path().string().find(".dagit") != std::string::npos)
        {
            continue;
        }

        if (!entry.is_regular_file())
        {
            continue;
        }

        std::filesystem::path relative =
            std::filesystem::relative(entry.path(), *repo);

        if (tracked_files.find(entry.path()) == tracked_files.end())
        {
            std::cout << "untracked: "
                      << relative.string()
                      << std::endl;

            has_changes = true;
        }
    }

    if (!has_changes)
    {
        std::cout << "working tree clean" << std::endl;
    }
}