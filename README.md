# DAGit

A lightweight Git-like version control system implemented in **C++17**.

DAGit is a learning-focused implementation of core Git concepts such as repositories, content-addressable object storage, SHA-1 object identification, trees, and commits. The project was built to understand how Git works internally rather than simply using Git commands.

## Features

### Repository Management

* Initialize a DAGit repository using `init`
* Creates a `.dagit` directory
* Creates an object store at `.dagit/objects`
* Automatically searches parent directories for the nearest DAGit repository

### Content-Addressable Object Storage

* Stores objects using SHA-1 object IDs
* Supports different object types:

  * `blob`
  * `tree`
  * `commit`
* Identical objects are reused instead of being stored multiple times
* Objects are stored using the format:

```text
type\0content
```

### Blob Objects

Files are stored as blob objects.

The SHA-1 object ID depends on the object's type and content, not its filename.

This means two files containing identical content can reference the same blob object.

### Tree Objects

DAGit recursively converts a directory structure into tree objects.

For example:

```text
project/
├── main.cpp
├── README.md
└── src/
    └── test.cpp
```

is represented conceptually as:

```text
tree
├── blob  <oid> main.cpp
├── blob  <oid> README.md
└── tree  <oid> src
    └── blob <oid> test.cpp
```

Changing a file creates a new blob and consequently changes the tree object representing its directory.

### Commit Objects

DAGit can create a commit object containing the tree associated with the current working directory and a commit message.

The current commit representation is:

```text
tree <tree_oid>

<commit message>
```

## Available Commands

### Initialize Repository

```bash
DAGit init
```

Creates a new `.dagit` repository in the current directory.

### Display Version

```bash
DAGit version
```

Displays the current DAGit version.

### Hash a File

```bash
DAGit hash-object <file>
```

Reads a file and stores it as a blob object using SHA-1 content addressing.

### Display an Object

```bash
DAGit cat-file <oid>
```

Reads an object from the DAGit object store and displays its contents.

### Write Working Tree

```bash
DAGit write-tree
```

Recursively converts the current working directory into tree objects and returns the root tree's object ID.

### Restore a Tree

```bash
DAGit read-tree <tree_oid>
```

Reads a tree object and restores the files represented by that tree into the working directory.

> **Warning:** The current implementation is destructive and can remove existing working-tree files. Do not use this command on files you need to preserve.

### Create a Commit

```bash
DAGit commit -m "Initial commit"
```

Creates a commit object using the current working tree and the supplied commit message.

## Project Structure

```text
DAGit/
│
├── main.cpp
├── CMakeLists.txt
│
├── include/
│   ├── commands/
│   │   ├── cat_file_command.hpp
│   │   ├── commit_command.hpp
│   │   ├── hash_object_command.hpp
│   │   ├── icommand.hpp
│   │   ├── init_command.hpp
│   │   ├── read_tree_command.hpp
│   │   ├── version_command.hpp
│   │   └── write_tree_command.hpp
│   │
│   └── core/
│       ├── commit.hpp
│       ├── object_store.hpp
│       ├── repository.hpp
│       └── tree.hpp
│
├── src/
│   ├── command/
│   │   ├── cat_file_command.cpp
│   │   ├── commit_command.cpp
│   │   ├── hash_object_command.cpp
│   │   ├── read_tree_command.cpp
│   │   └── write_tree_command.cpp
│   │
│   └── core/
│       ├── commit.cpp
│       ├── object_store.cpp
│       ├── repository.cpp
│       └── tree.cpp
│
└── external/
    └── CLI11/
```

## Architecture

DAGit is divided into two main layers.

### Command Layer

The command layer handles the CLI interface:

```text
init
version
hash-object
cat-file
write-tree
read-tree
commit
```

Commands implement the common `ICommand` interface.

### Core Layer

The core layer contains the actual version-control logic:

```text
Repository
    │
    ├── Repository discovery
    └── Repository initialization
     
ObjectStore
    │
    ├── SHA-1 hashing
    ├── Object storage
    ├── Object lookup
    └── Object retrieval

Tree
    │
    ├── Directory traversal
    ├── Tree creation
    ├── Tree traversal
    └── Working-tree restoration

Commit
    │
    └── Commit object creation
```

## Technologies Used

* **C++17**
* **CMake**
* **OpenSSL Crypto**
* **CLI11**
* `std::filesystem`
* `std::unordered_map`
* `std::map`
* C++ smart pointers
* SHA-1 hashing

## Building the Project

### Prerequisites

Make sure the following are installed:

* C++17-compatible compiler
* CMake 3.16+
* OpenSSL
* Git

### Build

Clone the repository:

```bash
git clone <your-repository-url>
cd DAGit
```

Create a build directory:

```bash
cmake -S . -B build
```

Build the project:

```bash
cmake --build build
```

The resulting executable will be generated inside the build directory.

## Example Workflow

Create a test directory:

```bash
mkdir test_repo
cd test_repo
```

Initialize DAGit:

```bash
DAGit init
```

Create a file:

```bash
echo "Hello DAGit" > hello.txt
```

Write the working tree:

```bash
DAGit write-tree
```

This produces a tree object ID.

Create a commit:

```bash
DAGit commit -m "Initial commit"
```

DAGit creates a commit object referencing the generated tree.

## How DAGit Represents Files

A file is stored as a blob:

```text
file content
      │
      ▼
  SHA-1 hash
      │
      ▼
  Blob object
```

A directory is represented by a tree:

```text
          Tree
        /      \
      Blob     Tree
       │       /  \
     file    Blob Blob
```

This creates a hierarchy of content-addressed objects.

A commit then points to the root tree:

```text
Commit
   │
   ▼
Root Tree
   │
   ├── Blob
   ├── Blob
   └── Tree
        ├── Blob
        └── Blob
```

## Design Principles

### Content Addressing

Objects are identified by the SHA-1 hash of their type and content.

Therefore:

```text
same type + same content
        ↓
same object ID
```

This naturally allows object reuse.

### Immutable Objects

Once an object is created, changing the content results in a new object ID rather than modifying the old object.

For example:

```text
file.txt
   │
   ▼
Blob A
```

After changing the file:

```text
file.txt
   │
   ▼
Blob B
```

`Blob A` still exists in the object store.

### Recursive Trees

Trees represent directory hierarchy recursively. A change deep inside a directory therefore changes the tree ID of that directory and propagates upward to the root tree.

## Current Limitations

DAGit is a learning implementation and is **not intended to be a drop-in replacement for Git**.

Current limitations include:

* No staging area/index
* No branches
* No HEAD/reference management
* No parent commit relationships
* No author/committer metadata
* No timestamps in commits
* No merge functionality
* No checkout implementation comparable to Git
* No object compression
* No Git-compatible object format
* No garbage collection
* Limited working-tree safety checks
* `read-tree` can overwrite/remove existing files
* Some command output and error handling are still basic

## Future Improvements

Possible future additions:

* Implement parent commits
* Add `HEAD` and branch references
* Implement a staging area/index
* Add `status`
* Add `log`
* Implement checkout
* Improve object format compatibility
* Add safer working-tree restoration
* Add commit metadata and timestamps
* Add branch management
* Add merge functionality
* Add automated tests

## Learning Goals

This project was developed to understand the internal mechanisms behind distributed version-control systems, particularly:

* Content-addressable storage
* SHA-1 hashing
* Git-style blob and tree concepts
* Recursive directory representation
* Object graphs
* Commit structures
* Repository discovery
* C++ filesystem operations
* C++ object-oriented design
* CLI application architecture
* CMake-based C++ projects

## License

This project is intended primarily as an educational project.
