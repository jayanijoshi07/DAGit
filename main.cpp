#include <memory>
#include <vector>
#include <optional>

#include "include/commands/init_command.hpp"
#include "include/commands/version_command.hpp"
#include "include/commands/hash_object_command.hpp"
#include "include/commands/add_command.hpp"
#include "include/commands/status_command.hpp"
#include "include/commands/cat_file_command.hpp"
#include "include/commands/write_tree_command.hpp"
#include "include/commands/read_tree_command.hpp"
#include "include/commands/commit_command.hpp"

int main(int argc, char* argv[])
{   
	CLI::App app{"DAGit"};
	
	app.require_subcommand(1);
	std::unique_ptr<ICommand> ver =std::make_unique<VersionCommand>();
	ver->setup(app);
											 
	std::unique_ptr<ICommand> init =std::make_unique<InitCommand>();
	init->setup(app);
	                                           
	std::unique_ptr<ICommand> hash = std::make_unique<HashObjectCommand>();
	hash->setup(app);

	std::unique_ptr<ICommand> add_s = std::make_unique<AddCommand>();
	add_s->setup(app);

	std::unique_ptr<ICommand> status_c = std::make_unique<StatusCommand>();
	status_c->setup(app);

	std::unique_ptr<ICommand> cat_file = std::make_unique<CatFileCommand>();
	cat_file->setup(app);

	std::unique_ptr<ICommand> write_tree = std::make_unique<WriteTreeCommand>();
	write_tree->setup(app);

	std::unique_ptr<ICommand> read_tree = std::make_unique<ReadTreeCommand>();
	read_tree->setup(app);

	std::unique_ptr<ICommand> commit = std::make_unique<CommitCommand>();
	commit->setup(app);
	
	CLI11_PARSE(app, argc, argv);
	//std::cout << "after  " << file_path << std::endl;
	return 0;
}

	
	
