#include "../../include/commands/write_tree_command.hpp"
#include "../../include/core/tree.hpp"
#include "../../include/core/repository.hpp"

  

void WriteTreeCommand::setup(CLI::App& app) {
    auto cmd = app.add_subcommand("write-tree", "Write repository tree");
    cmd->callback([&](){
        execute();
    });
}

void WriteTreeCommand::execute(){
    auto repo =core::Repository::find_repo_root();

    if (!repo)
    {
        std::cerr<< "Not inside a DAGit repository"<< std::endl;
        return;
    }
    
    try
    {
        std::string oid =core::Tree::write_tree();
        std::cout<< oid << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr<< e.what()<< std::endl;
    }

}
