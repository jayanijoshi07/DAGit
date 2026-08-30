cmake_minimum_required(VERSION 3.16)

project(DAGitPractice)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(DAGitPractice
    main.cpp
    src/command/hash_object_command.cpp
    src/command/cat_file_command.cpp
    src/command/write_tree_command.cpp
    src/command/read_tree_command.cpp
    src/command/commit_command.cpp
    src/core/repository.cpp
	src/core/object_store.cpp
    src/core/tree.cpp
	src/core/commit.cpp
)

target_include_directories(DAGitPractice PRIVATE
    include
	"${CMAKE_CURRENT_SOURCE_DIR}/external/CLI"
)

# OpenSSL
find_package(OpenSSL REQUIRED)

target_link_libraries(
    DAGitPractice PRIVATE
    OpenSSL::Crypto
)