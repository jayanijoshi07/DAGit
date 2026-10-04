#pragma once

#include <string>

#include "./icommand.hpp"

class CheckoutCommand : public ICommand
{
public:
    void setup(CLI::App& app) override;
    void execute();

private:
    std::string branch_name_;
};