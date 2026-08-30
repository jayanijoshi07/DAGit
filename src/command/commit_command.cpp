#include <iostream>
#include "../../include/commands/commit_command.hpp"
#include "../../include/core/commit.hpp"




void CommitCommand::execute(){
    std::cout << "CommitCommand::execute() " << message << std::endl;
    auto oid = core::Commit::create_commit(message);
    std::cout <<  "\ncommit oid " << oid << std::endl; 
}

void CommitCommand::setup(CLI::App& app) {
    auto cmd = app.add_subcommand("commit","Create commit");
    cmd->add_option("-m,--message",message,"Commit message")->required();
    cmd->callback([&](){
        execute();
    });
}