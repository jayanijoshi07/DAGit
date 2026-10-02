#pragma once

#include "./icommand.hpp"

class StatusCommand : public ICommand
{
private:
    std::string file_path_;

public:
    void setup(CLI::App& app) override;
    void execute();
};