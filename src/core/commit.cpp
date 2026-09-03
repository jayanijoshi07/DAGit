#include <iostream>
#include "../../include/core/commit.hpp"
#include "../../include/core/repository.hpp"
#include "../../include/core/tree.hpp"
#include "../../include/core/object_store.hpp"


namespace core{
std::string Commit::create_commit(const std::string &message){
    std::cout << "create_commit of Commit class" << message << std::endl;
    auto repo = Repository::find_repo_root();

	if (!repo)
	{
		throw std::runtime_error("Not inside a DAGit repository");
	}
	
	std::string tree_oid = Tree::write_tree();
	
	std::string commit_data =  "tree " + tree_oid + "\n\n" + message; 
	
	 
	
	std::string commit_oid = ObjectStore::hash_object(commit_data,"commit");
    return commit_oid;
}
} // core
