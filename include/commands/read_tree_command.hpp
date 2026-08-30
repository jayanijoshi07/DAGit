#pragma once
#include <string>
#include ".\icommand.hpp"


class ReadTreeCommand : public ICommand {
private :
    void execute();
    std::string oid;
public :
    
    void setup(CLI::App& app) override;
            
};