#pragma once
#include<string>
namespace core {

class Commit {
public :
    static std::string create_commit(const std::string &message);
}; // Commit
} // core