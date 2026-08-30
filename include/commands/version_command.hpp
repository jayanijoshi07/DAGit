#pragma once

#include <iostream>

#include ".\icommand.hpp"
class VersionCommand : public ICommand
{
public:
    void setup(CLI::App& app) override {
        auto cmd = app.add_subcommand("version", "Display DAGit Version");
        cmd->callback([](){
            std::cout << "DAGit version 1" << std::endl;
        });
    }
    
};