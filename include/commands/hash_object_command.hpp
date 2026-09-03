#pragma once 

#include <string>

#include "./icommand.hpp"

class HashObjectCommand : public ICommand{
public:
    
    void setup(CLI::App& app) override ;
private:
    void execute();
    std::string file_path_;
};
