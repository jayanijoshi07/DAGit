#pragma once
#include <string>
#include ".\icommand.hpp"
class CatFileCommand : public ICommand{
private :
    std::string oid;
    void execute();
public :
    
    void setup(CLI::App& app) override;
    
};