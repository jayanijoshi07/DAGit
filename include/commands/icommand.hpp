#pragma once
#include <CLI.hpp>

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void setup(CLI::App& app)=0;
    
};