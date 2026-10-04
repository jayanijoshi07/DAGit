#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

#include "../../include/commands/log_command.hpp"
#include "../../include/core/repository.hpp"
#include "../../include/core/object_store.hpp"

void LogCommand::setup(CLI::App& app)
{
    auto cmd = app.add_subcommand("log", "Show commit history");

    cmd->callback([&]()
    {
        execute();
    });
}

void LogCommand::execute()
{
    auto repo = core::Repository::find_repo_root();

    if(!repo)
    {
        throw std::runtime_error("Not inside a DAGit repository");
    }

    std::filesystem::path head_path = *repo / ".dagit" / "HEAD";

    std::ifstream head_in(head_path);

    if(!head_in)
    {
        throw std::runtime_error("Failed to open HEAD");
    }

    std::string branch_name;
    std::getline(head_in, branch_name);

    std::filesystem::path branch_path = *repo / ".dagit" / "refs" / "heads" / branch_name;

    std::ifstream branch_in(branch_path);

    if(!branch_in)
    {
        throw std::runtime_error("No commits yet");
    }

    std::string commit_oid;
    std::getline(branch_in, commit_oid);

    while(!commit_oid.empty())
    {
        std::string commit_data = core::ObjectStore::get_object(commit_oid, "commit");

        std::stringstream ss(commit_data);

        std::string line;
        std::string parent_oid;
        std::string message;

        while(std::getline(ss, line))
        {
            if(line.find("parent ", 0) == 0)
            {
                parent_oid = line.substr(7);
            }
            else if(line.find("message ", 0) == 0)
            {
                message = line.substr(8);
            }
        }

        std::cout << "commit " << commit_oid << std::endl;
        std::cout << "message: " << message << std::endl;
        std::cout << std::endl;

        commit_oid = parent_oid;
    }
}