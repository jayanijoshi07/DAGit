#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include "../../include/core/tree.hpp"
#include "../../include/core/object_store.hpp"
#include "../../include/core/repository.hpp"

                                               
                                               
namespace core
{
static std::string build_tree(
    const std::filesystem::path& directory,
    const std::map<std::filesystem::path, std::string>& staged_files)
{
    std::vector<std::string> entries;

    for (const auto &[file_path, oid] : staged_files)
    {
        if (file_path.parent_path() == directory)
        {
            entries.push_back(
                "blob " + oid + " " + file_path.filename().string()
            );
        }
    }

    std::set<std::filesystem::path> subdirectories;

    for (const auto &[file_path, oid] : staged_files)
    {
        auto parent = file_path.parent_path();

        if (parent != directory && !parent.empty())
        {
            auto relative = std::filesystem::relative(parent, directory);

            if (!relative.empty())
            {
                auto first_directory = relative.begin();

                if (*first_directory != "..")
                {
                    subdirectories.insert(
                        directory / *first_directory
                    );
                }
            }
        }
    }

    for (const auto &subdirectory : subdirectories)
    {
        std::string subtree_oid =
            build_tree(subdirectory, staged_files);

        entries.push_back(
            "tree " + subtree_oid + " " +
            subdirectory.filename().string()
        );
    }

    std::sort(entries.begin(), entries.end());

    std::string tree_data;

    for (const auto &entry : entries)
    {
        tree_data += entry + '\n';
    }

    return ObjectStore::hash_object(tree_data, "tree");
}//build_tree
std::string Tree::write_tree(const std::filesystem::path &path )
{
    auto repo = Repository::find_repo_root();

    if (!repo)
    {
        throw std::runtime_error("Not inside DAGit repository");
    }

    std::filesystem::path index_path = *repo / ".dagit" / "index";

    std::ifstream index(index_path);

    if (!index)
    {
        throw std::runtime_error("Failed to open index");
    }

    std::cout << "Tree::write_tree" << path << std::endl;

    std::map<std::filesystem::path, std::string> staged_files;

    std::string line;

    while (std::getline(index, line))
    {
        std::stringstream ss(line);

        std::string file_path;
        std::string oid;

        ss >> file_path >> oid;

        staged_files[file_path] = oid;
    }

    return build_tree(std::filesystem::path(), staged_files);
} // std::string Tree::write_tree(const std::filesystem::path &path )
void Tree::get_tree(const std::string &tree_oid,const std::filesystem::path &base_path,std::map<std::filesystem::path,std::string> &entries)
{
    std::string tree_content =ObjectStore::get_object(tree_oid,"tree");
        
    std::stringstream ss(tree_content);

    std::string line;
    
    while (std::getline(ss, line))
    {
        std::stringstream line_stream(line);

        std::string type;
        std::string oid;
        std::string name;

        line_stream >> type >> oid >> name;
            
        std::filesystem::path full_path = base_path / name;
        
        if (type == "blob")
        {
            entries[full_path] = oid;
        }
        else if (type == "tree")
        {
            get_tree(oid,full_path,entries);
        }
    }
} // end of void Tree::get_tree
void Tree::read_tree(const std::string &tree_oid){
    auto repo_root =Repository::find_repo_root();
	if (!repo_root)
	{
		throw std::runtime_error("Not inside a DAGit repository");
	}
    std::map<std::filesystem::path,std::string> entries;
    get_tree(tree_oid,"",entries);
    std::vector<std::filesystem::path> paths_to_remove;
		
    for (const auto &entry :std::filesystem::recursive_directory_iterator(*repo_root))
    {
        // Ignore .dagit
        if (entry.path().string().find(".dagit")!= std::string::npos)
        {
            continue;
        }
        
        paths_to_remove.push_back(entry.path());
    }
    
    std::sort(paths_to_remove.begin(),paths_to_remove.end(),[](const std::filesystem::path &a,const std::filesystem::path &b)
        {
            return a.string().size()
                    > b.string().size();
        }
    );
    
    for (const auto &path : paths_to_remove)
    {
        std::filesystem::remove(path);
    }
    
    for (const auto &[path, oid] : entries)
    {
        std::filesystem::path full_path = *repo_root / path;
        
        std::filesystem::create_directories(full_path.parent_path());
        
        std::string content = ObjectStore::get_object(oid,"blob");
        
        std::ofstream out(full_path,std::ios::binary);
        
        if (!out)
        {
            throw std::runtime_error("Failed to write file: " + full_path.string());
        }
        
        out << content;
    }

}//read-tree
} // core