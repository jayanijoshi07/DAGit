#pragma once

#include "./icommand.hpp"

class LogCommand : public ICommand
{
public:
    void setup(CLI::App& app) override;
    void execute();
};