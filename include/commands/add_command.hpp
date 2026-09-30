#pragma once
#include <string>
#include "./icommand.hpp"

class AddCommand : public ICommand {
private : 
    std::string file_path_;
    void execute();
public :
    void setup(CLI::App& app);
    
};