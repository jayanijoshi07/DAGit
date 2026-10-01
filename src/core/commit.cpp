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
std::string Commit::create_commit(const std::string &message)
{
    auto repo = Repository::find_repo_root();

    if (!repo)
    {
        throw std::runtime_error("Not inside a DAGit repository");
    }

    std::string tree_oid = Tree::write_tree();

    std::filesystem::path head_path = *repo / ".dagit" / "HEAD";

    std::string parent_oid;

    std::ifstream head_in(head_path);

    if (head_in)
    {
        std::getline(head_in, parent_oid);
    }

    std::string commit_data;

    commit_data += "tree " + tree_oid + "\n";
    commit_data += "author Jayani Joshi\n";

    if (!parent_oid.empty())
    {
        commit_data += "parent " + parent_oid + "\n";
    }

    commit_data += "message " + message + "\n";

    std::string commit_oid =
        ObjectStore::hash_object(commit_data, "commit");

    std::ofstream head_out(head_path, std::ios::trunc);

    if (!head_out)
    {
        throw std::runtime_error("Failed to update HEAD");
    }

    head_out << commit_oid << "\n";

    return commit_oid;
}
}