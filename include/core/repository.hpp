#pragma once
#include <string>
#include <filesystem>
#include <optional>
namespace core {
class Repository {
public:
     static bool init(const std::string& path = ".");
     static std::optional<std::filesystem::path> find_repo_root(const std::filesystem::path& start =
            std::filesystem::current_path());
};// Repository
} // core