#pragma once
#include <string>
#include <filesystem>
#include <optional>
#include <map>

namespace core {
class Tree {
public:
     static std::string write_tree(const std::filesystem::path &path = ".");
     static void read_tree(const std::string &tree_oid);
     static void get_tree(const std::string &tree_oid,const std::filesystem::path &base_path,std::map<std::filesystem::path,std::string> &entries);
                                
};// Tree
} // core