#include <iostream>
#include "../../include/commands/cat_file_command.hpp"
#include "../../include/core/repository.hpp"
#include "../../include/core/object_store.hpp"

void CatFileCommand::setup(CLI::App& app) {
    auto cmd = app.add_subcommand("cat-file","Display object contents");
    cmd->add_option("oid" , oid , "Object ID")->required();
    cmd->callback([&](){
        execute();
    });
}
    
void CatFileCommand::execute(){
    
    auto repo =	core::Repository::find_repo_root();

    if (!repo)
    {
        std::cout<< "Not inside a DAGit repository"<< std::endl;
        return;
    }

    auto exists = core::ObjectStore::object_exists(oid);
    std::cout << "exists : " << exists << std::endl;
    if(exists){
        std::string content =core::ObjectStore::get_object(oid,std::nullopt);

        std::cout<< content<< std::endl;
    }
}