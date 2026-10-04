#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

#include "../../include/core/commit.hpp"
#include "../../include/core/repository.hpp"
#include "../../include/core/tree.hpp"
#include "../../include/core/object_store.hpp"

namespace core
{

std::string Commit::create_commit(const std::string& message)
{
    auto repo = Repository::find_repo_root();

    if(!repo)
    {
        throw std::runtime_error("Not inside a DAGit repository");
    }

    std::filesystem::path head_path = *repo / ".dagit" / "HEAD";

    std::ifstream head_in(head_path);

    if(!head_in)
    {
        throw std::runtime_error("Failed to open HEAD");
    }

    std::string branch_name;
    std::getline(head_in, branch_name);

    if(branch_name.empty())
    {
        throw std::runtime_error("Invalid HEAD");
    }

    std::filesystem::path branch_path = *repo / ".dagit" / "refs" / "heads" / branch_name;

    std::string parent_oid;

    std::ifstream branch_in(branch_path);

    if(branch_in)
    {
        std::getline(branch_in, parent_oid);
    }

    std::string tree_oid = Tree::write_tree();

    std::string commit_data;

    commit_data += "tree " + tree_oid + "\n";
    commit_data += "author Jayani Joshi\n";

    if(!parent_oid.empty())
    {
        commit_data += "parent " + parent_oid + "\n";
    }

    commit_data += "message " + message + "\n";

    std::string commit_oid = ObjectStore::hash_object(commit_data, "commit");

    std::ofstream branch_out(branch_path, std::ios::trunc);

    if(!branch_out)
    {
        throw std::runtime_error("Failed to update branch");
    }

    branch_out << commit_oid << "\n";

    return commit_oid;
}

}