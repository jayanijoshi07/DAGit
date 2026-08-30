#pragma once
#include <string>
#include <optional>

namespace core {

class ObjectStore {
public :
    static std::string hash_object(const std::string& data,const std::string& type);
    static bool object_exists(const std::string& oid);
    static std::string get_object(const std::string& oid,const std::optional<std::string>& expected_type =std::nullopt);
}; // ObjectStore

    

} // core