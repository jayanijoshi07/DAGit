#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <vector>

#include "../../include/commands/add_command.hpp"
#include "../../include/core/repository.hpp"
#include "../../include/core/object_store.hpp"

void AddCommand::setup(CLI::App& app){
    auto cmd = app.add_subcommand("add","Add file to staging area");
    cmd->add_option("file",file_path_)->required();
    cmd->callback([&](){
        execute();
    });
}

void AddCommand::execute()
{
    auto repo = core::Repository::find_repo_root();

    if (!repo)
    {
        throw std::runtime_error("Not inside DAGit repository");
    }
    std::filesystem::path file =std::filesystem::current_path() / file_path_;

    if (!std::filesystem::exists(file))
    {
        throw std::runtime_error("File does not exist: " + file_path_);
    }

    std::ifstream in(file, std::ios::binary);

    if (!in)
    {
        throw std::runtime_error("Failed to open file: " + file_path_);
    }

    std::stringstream buffer;
    buffer << in.rdbuf();

    std::string content = buffer.str();
    std::string oid = core::ObjectStore::hash_object(content, "blob");
    std::filesystem::path index_path = *repo / ".dagit" / "index";

    std::ifstream index_in(index_path);

    if (!index_in)
    {
        throw std::runtime_error("Failed to open index");
    }

    std::vector<std::string> entries;
    std::string line;

    while (std::getline(index_in, line))
    {
        std::stringstream ss(line);

        std::string path;
        std::string old_oid;

        ss >> path >> old_oid;

        if (path != file_path_)
        {
            entries.push_back(line);
        }
    }
    std::ofstream index(index_path, std::ios::trunc);

    if (!index)
    {
        throw std::runtime_error("Failed to open index");
    }

    for (const auto &entry : entries)
    {
        index << entry << "\n";
    }

    index << file_path_ << " " << oid << "\n";
        
}