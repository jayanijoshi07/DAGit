# DAGit

DAGit is a lightweight Git-like version control system implemented in **C++17** as part of the WEC Systems assignment.

The project implements the core ideas behind Git, including content-addressable objects, a staging area, trees, commits, branches, checkout, and commit history.

## Features

* Repository initialization
* SHA-256 based object identification
* Blob object storage
* Staging area (index)
* Tree objects
* Commit objects
* Branch creation
* Branch checkout
* Commit history
* Working-tree status
* Reading and restoring tree snapshots

## Architecture

DAGit's repository state can be understood as:

```text
Working Directory
       |
     add
       v
     Index
       |
  write-tree
       v
     Tree
       |
    commit
       v
    Commit
       |
       v
Branch Reference
       ^
       |
      HEAD
```

### Working Directory

Contains the actual files being edited by the user.

### Index

The staging area stores the repository-relative file path and the SHA-256 object ID of the staged blob.

Example:

```text
main.cpp abc123...
src/a.cpp 98fedc...
```

### Objects

DAGit stores objects using their SHA-256 object IDs. The main object types are:

* **Blob** — stores file contents
* **Tree** — stores a directory snapshot and references blobs/subtrees
* **Commit** — stores a tree reference, parent commit, author, and message

### Commits

A commit points to a tree and, except for the first commit, its parent commit.

```text
C3
 |
C2
 |
C1
```

This parent relationship allows DAGit to walk backward through history.

### Branches

A branch is a reference to the latest commit.

For example:

```text
refs/heads/main    -> C3
refs/heads/feature -> C2
```

Creating a branch does not copy files or objects. It creates another reference to an existing commit.

### HEAD

`HEAD` stores the name of the currently checked-out branch.

Example:

```text
HEAD
 |
 +-- main
      |
      +-- refs/heads/main
              |
              +-- C3
```

Therefore:

```text
HEAD -> current branch -> latest commit
```

## Repository Structure

After running `DAGit init`, the repository contains:

```text
.dagit/
├── objects/
├── index
├── HEAD
└── refs/
    └── heads/
```

* `objects/` — stores blob, tree, and commit objects
* `index` — staging area
* `HEAD` — current branch name
* `refs/heads/` — branch references

## Commands

### `init`

Initializes a new DAGit repository.

```bash
DAGit init
```

Creates the `.dagit` directory, object store, index, `HEAD`, and branch-reference directory.

---

### `add`

Stages a file.

```bash
DAGit add main.cpp
DAGit add src/a.cpp
```

The command:

1. Reads the file.
2. Creates a blob object using its contents.
3. Calculates its SHA-256 object ID.
4. Updates the index with the file path and blob ID.

Example index:

```text
main.cpp abc123...
src/a.cpp 98fedc...
```

---

### `write-tree`

Creates a tree object from the current index.

```bash
DAGit write-tree
```

The tree represents the staged directory structure.

---

### `read-tree`

Restores files from a tree object.

```bash
DAGit read-tree <tree-oid>
```

It restores the working directory and updates the index to match the tree.

---

### `commit`

Creates a commit from the current staged state.

```bash
DAGit commit "Initial commit"
```

The commit process is:

```text
HEAD
  |
current branch
  |
previous commit
  |
index
  |
write-tree
  |
new tree
  |
new commit
  |
update branch reference
```

A commit contains:

```text
tree <tree-oid>
author <author>
parent <parent-oid>    # except for the first commit
message <message>
```

The current branch reference is then updated to the new commit.

---

### `branch`

Creates a new branch from the current commit.

```bash
DAGit branch feature
```

If:

```text
main -> C2
```

then:

```text
main    -> C2
feature -> C2
```

Creating a branch does **not** switch to it.

---

### `checkout`

Switches to another branch.

```bash
DAGit checkout feature
```

The checkout process is:

```text
feature branch
      |
      v
latest commit
      |
      v
commit's tree
      |
      v
read-tree
      |
      +--> working directory
      |
      +--> index
      |
      v
HEAD = feature
```

The working directory and index are therefore updated to match the checked-out branch.

---

### `status`

Shows changes between the working directory and the staged state.

```bash
DAGit status
```

It can report:

```text
modified: file.cpp
deleted: file.cpp
untracked: new.cpp
```

If there are no changes:

```text
working tree clean
```

---

### `log`

Displays the commit history of the current branch.

```bash
DAGit log
```

The command starts from:

```text
HEAD
  |
current branch
  |
latest commit
```

and follows the parent references:

```text
C3 -> C2 -> C1
```

It therefore shows the history reachable from the currently checked-out branch, not commits from unrelated branches.

## Example Workflow

Initialize a repository:

```bash
DAGit init
```

Stage a file:

```bash
DAGit add main.cpp
```

Create the first commit:

```bash
DAGit commit "Initial commit"
```

Create a branch:

```bash
DAGit branch feature
```

Switch to it:

```bash
DAGit checkout feature
```

Make changes and stage them:

```bash
DAGit add main.cpp
```

Commit the changes:

```bash
DAGit commit "Added feature"
```

View the current branch history:

```bash
DAGit log
```

Switch back:

```bash
DAGit checkout main
```

## Branch Model

DAGit uses lightweight branch references.

For example:

```text
                 C1
                /  \
               /    \
            main   feature
              |       |
              C2      C3
              ^
              |
             HEAD
```

When `HEAD` points to `main`:

```text
HEAD -> main -> C2
```

When checking out `feature`:

```text
HEAD -> feature -> C3
```

The commits themselves are not copied. Only the branch reference and HEAD change.

## Object Model

DAGit uses content-addressable storage.

```text
File contents
     |
     v
SHA-256
     |
     v
Blob OID
```

Trees reference blobs:

```text
Tree
├── main.cpp -> Blob A
└── src/
    └── a.cpp -> Blob B
```

Commits reference trees:

```text
Commit
├── tree -> Tree
├── parent -> Previous Commit
├── author
└── message
```

This creates the history:

```text
Commit C3
    |
    v
Commit C2
    |
    v
Commit C1
```

## Technologies

* **C++17**
* **CMake**
* **CLI11** for command-line argument parsing
* **OpenSSL SHA-256** for object hashing
* `std::filesystem` for repository and file operations
* STL containers and streams for repository data management

## Project Goals

The purpose of DAGit is to understand the internal design of a distributed version control system rather than simply use Git commands.

The implementation focuses on:

* File and object management
* Content-addressable storage
* Staging and snapshots
* Commit history
* References and branches
* Filesystem operations
* C++ systems programming concepts
