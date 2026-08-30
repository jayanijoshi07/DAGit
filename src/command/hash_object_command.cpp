#include <iostream>
#include <fstream>


#include "../../include/commands/hash_object_command.hpp"
#include "../../include/core/repository.hpp"
#include "../../include/core/object_store.hpp"

void HashObjectCommand::setup(CLI::App& app) {
    auto cmd = app.add_subcommand("hash-object","Hash a file into DAGit object store");
    cmd->add_option("file_path_",file_path_,"File to hash")->required();
    cmd->callback([&](){
        execute();
    });
}


void HashObjectCommand::execute() {
    
    auto repo = core::Repository::find_repo_root();

    if (!repo)
    {
        //throw std::runtime_error(
        //    "Not inside DAGit repository");
        std::cout << "Not inside DAGit repository" << std::endl;
        return;
    }
    
    std::ifstream in(file_path_,std::ios::binary);

    if (!in)
    {
        throw std::runtime_error("Failed to open file");
    }
    std::stringstream buffer;

    buffer << in.rdbuf();

    std::string data = buffer.str();
   
    std::string oid = core::ObjectStore::hash_object(data,"blob");
    
}