#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include "../../include/core/tree.hpp"
#include "../../include/core/object_store.hpp"
#include "../../include/core/repository.hpp"

                                               
                                               
namespace core
{
std::string Tree::write_tree(const std::filesystem::path &path ){
                                               
    std::cout << "Tree::write_tree" << path << std::endl;
    std::vector<std::string> entries;
    for (const auto &entry : std::filesystem::directory_iterator(path)){
        if (entry.path().filename() == ".dagit" || entry.path().filename() == "DAGitPractice.exe")
		{
			continue;
		}
        if (std::filesystem::is_regular_file(entry))
		{   
            std::cout << entry << std::endl;
			std::ifstream in(entry.path(),std::ios::binary);
            if (!in)
			{
				throw std::runtime_error("Failed to open file: " +entry.path().string());
			}
            std::stringstream buffer;
			buffer << in.rdbuf();

			std::string content =buffer.str();
			std::string oid =ObjectStore::hash_object(content,"blob");
            entries.push_back("blob " +oid +" " +entry.path().filename().string());

            std::cout << oid << std::endl;
            

        }
        else if (std::filesystem::is_directory(entry))
		{
			std::string oid = write_tree(entry.path());
			entries.push_back("tree " +oid +" " + entry.path().filename().string());
		}
    }//for
    std::sort(entries.begin(),entries.end());
    std::string tree_data = "";
    for (const auto &e : entries)
	{
		tree_data = tree_data + e + '\n';
    }
    std::string ans = ObjectStore::hash_object(tree_data,"tree");
    return ans;

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