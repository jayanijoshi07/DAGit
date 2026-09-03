#pragma once
#include <iostream>

#include "./icommand.hpp"

class WriteTreeCommand : public ICommand{

public :
    
    void setup(CLI::App& app) override ;

private :
    void execute();
        
    
    
};
