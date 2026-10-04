#include <iostream>
#include <fstream>
#include <filesystem>
#include <stdexcept>

#include "../../include/commands/branch_command.hpp"
#include "../../include/core/repository.hpp"

void BranchCommand::setup(CLI::App& app)
{
    auto cmd = app.add_subcommand("branch", "Create a new branch");

    cmd->add_option("name", branch_name_)->required();

    cmd->callback([&]()
    {
        execute();
    });
}

void BranchCommand::execute()
{
    auto repo = core::Repository::find_repo_root();

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

    std::string current_branch;
    std::getline(head_in, current_branch);

    std::filesystem::path current_branch_path = *repo / ".dagit" / "refs" / "heads" / current_branch;

    std::ifstream branch_in(current_branch_path);

    if(!branch_in)
    {
        throw std::runtime_error("No commits yet");
    }

    std::string current_commit;
    std::getline(branch_in, current_commit);

    if(current_commit.empty())
    {
        throw std::runtime_error("No commits yet");
    }

    std::filesystem::path new_branch_path = *repo / ".dagit" / "refs" / "heads" / branch_name_;

    if(std::filesystem::exists(new_branch_path))
    {
        throw std::runtime_error("Branch already exists");
    }

    std::ofstream branch_out(new_branch_path);

    if(!branch_out)
    {
        throw std::runtime_error("Failed to create branch");
    }

    branch_out << current_commit << "\n";

    std::cout << "Created branch " << branch_name_ << std::endl;
}