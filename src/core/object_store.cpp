#include <iostream>

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <openssl/sha.h>
#include <sstream>
#include <stdexcept>

#include "../../include/core/object_store.hpp"
#include "../../include/core/repository.hpp"

namespace core
{
std::string sha1_hex(const std::string& data)
{
    unsigned char hash[SHA_DIGEST_LENGTH];

    SHA1(reinterpret_cast<const unsigned char*>(data.c_str()),data.size(),hash);

    std::stringstream ss;

    for(int i = 0;i < SHA_DIGEST_LENGTH;++i)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }

    return ss.str();
} // std::string sha1_hex(const std::string& data)

std::string ObjectStore::hash_object(const std::string& data,const std::string& type)
{
    

    auto repo = Repository::find_repo_root();

    if (!repo) {
        std::cout << "Not inside DAGit repository" << std::endl;
        return "";
    }

    std::string object = type + '\0' + data;

    std::string oid = sha1_hex(object);

    std::filesystem::path objectPath =repo.value() / ".dagit" / "objects" / oid;

    if(std::filesystem::exists(objectPath))
    {
        std::cout << oid << " Already exisits" << std::endl;
        return oid;
    }

    std::ofstream out(objectPath,std::ios::binary);

    
    if(!out)
    {
        throw std::runtime_error("Failed to write object");
    }

    
    out.write(object.data(),object.size());

    return oid;
} // std::string ObjectStore::hash_object(

bool ObjectStore::object_exists(const std::string& oid){
    auto repo_root =Repository::find_repo_root();

	if (!repo_root)
	{
		throw std::runtime_error("Not inside a DAGit repository");
	}
	
	std::filesystem::path object_path =*repo_root / ".dagit" / "objects" / oid;
	
	return std::filesystem::exists(object_path);
} // bool ObjectStore::object_exists(const std::string& oid){

std::string ObjectStore::get_object(const std::string& oid,const std::optional<std::string>& expected_type)
{
	auto repo_root = Repository::find_repo_root();
	
	if (!repo_root)
	{
		throw std::runtime_error("Not inside a DAGit repository");
	}
	
	std::filesystem::path object_path =*repo_root /".dagit" /"objects" /oid;
	
	if (!std::filesystem::exists(object_path))
	{
		throw std::runtime_error("Object does not exist");
	}
	
	std::ifstream in(object_path,std::ios::binary);
	
	if (!in)
	{
		throw std::runtime_error("Failed to open object");
	}
    
	std::stringstream buffer;
	buffer << in.rdbuf();

	std::string raw = buffer.str();
	
	size_t pos = raw.find('\0');
	
	if (pos == std::string::npos)
	{
		throw std::runtime_error("Corrupted object format");
	}
	
	std::string actual_type =raw.substr(0, pos);

	std::string content = raw.substr(pos + 1);
	
	if (expected_type && actual_type != *expected_type)
	{
		throw std::runtime_error("Object type mismatch");
	}
	
	return content;
}

} // namespace core
