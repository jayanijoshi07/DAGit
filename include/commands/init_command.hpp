#pragma once
#include <iostream>
                               
#include ".\icommand.hpp"
#include "../core/repository.hpp"
class InitCommand : public ICommand{
public:
	
    void setup(CLI::App& app) override {
        auto cmd = app.add_subcommand("init","Initialise DAGit repository");
        cmd->callback([](){
            core::Repository::init();
        });
    }
};