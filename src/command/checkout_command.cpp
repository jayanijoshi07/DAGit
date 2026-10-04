#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <string>
#include <stdexcept>

#include "../../include/commands/checkout_command.hpp"
#include "../../include/core/repository.hpp"
#include "../../include/core/object_store.hpp"
#include "../../include/core/tree.hpp"

void CheckoutCommand::setup(CLI::App& app)
{
    auto cmd = app.add_subcommand("checkout", "Switch branches");

    cmd->add_option("branch", branch_name_)->required();

    cmd->callback([&]()
    {
        execute();
    });
}

void CheckoutCommand::execute()
{
    auto repo = core::Repository::find_repo_root();

    if(!repo)
    {
        throw std::runtime_error("Not inside a DAGit repository");
    }

    std::filesystem::path branch_path = *repo / ".dagit" / "refs" / "heads" / branch_name_;

    if(!std::filesystem::exists(branch_path))
    {
        throw std::runtime_error("Branch does not exist: " + branch_name_);
    }

    std::ifstream branch_in(branch_path);

    if(!branch_in)
    {
        throw std::runtime_error("Failed to open branch");
    }

    std::string commit_oid;
    std::getline(branch_in, commit_oid);

    if(commit_oid.empty())
    {
        throw std::runtime_error("Branch has no commits");
    }

    std::string commit_data = core::ObjectStore::get_object(commit_oid, "commit");

    std::stringstream ss(commit_data);

    std::string line;
    std::string tree_oid;

    while(std::getline(ss, line))
    {
        if(line.find("tree ", 0) == 0)
        {
            tree_oid = line.substr(5);
            break;
        }
    }

    if(tree_oid.empty())
    {
        throw std::runtime_error("Invalid commit object");
    }

    core::Tree::read_tree(tree_oid);

    std::filesystem::path head_path = *repo / ".dagit" / "HEAD";

    std::ofstream head_out(head_path, std::ios::trunc);

    if(!head_out)
    {
        throw std::runtime_error("Failed to update HEAD");
    }

    head_out << branch_name_ << "\n";

    std::cout << "Switched to branch " << branch_name_ << std::endl;
}