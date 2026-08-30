#include <iostream>
#include "../../include/commands/read_tree_command.hpp"
#include "../../include/core/tree.hpp"




void ReadTreeCommand::setup(CLI::App& app) {
    auto cmd = app.add_subcommand("read-tree","Restore repository tree");
    cmd->add_option("tree_oid",oid,"Tree object ID")->required();
    cmd->callback([&](){
        execute();
    });
}

void ReadTreeCommand::execute(){
    std::cout << "in the execute of ReadTreeCommand" << std::endl;
    core::Tree::read_tree(oid);
    
}