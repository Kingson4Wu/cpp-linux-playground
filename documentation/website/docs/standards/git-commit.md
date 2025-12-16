---
sidebar_position: 3
---

# Git Commit Standards

Maintaining clear and consistent commit history is crucial for project maintainability and collaboration. This document outlines the Git commit standards for the Linux C++ Backend Development Playground project.

## Commit Message Format

Each commit message should follow this format:

```
<type>(<scope>): <subject>

<body>

<footer>
```

### Type

The commit type indicates the nature of the change:

- `feat`: New feature or functionality
- `fix`: Bug fix
- `bug`: Bug fix (synonymous with `fix`)
- `docs`: Documentation updates
- `style`: Code formatting changes (whitespace, formatting, etc.) that don't affect meaning
- `refactor`: Code changes that neither fix bugs nor add features
- `perf`: Performance improvements
- `test`: Adding or modifying tests
- `build`: Build system or external dependency changes
- `ci`: CI configuration files and script changes
- `chore`: Other changes that don't modify src or test files
- `revert`: Reverting a previous commit

### Scope (Optional)

Scope identifies the module or component affected by the commit:

- `cli-tools`: Command-line tools (ls, grep, wc)
- `json-parser`: JSON parser implementation
- `logger`: Logging system
- `memory-pool`: Memory pool allocator
- `tcp-chat`: TCP chat room implementation
- `http-server`: HTTP server implementation
- `crawler`: Web crawler functionality
- `redis`: mini-Redis implementation
- `cmake`: CMake build files
- `docker`: Docker configuration
- `tests`: Test files and configurations

### Subject Line

- Start with a verb in present tense: "Add", "Fix", "Update", "Remove", etc.
- Keep under 72 characters
- Be specific but concise
- Don't end with a period

### Body (Optional)

- Explain what changes were made and why
- Provide context for the changes
- Separate from subject by a blank line
- Wrap lines at 72 characters
- Use bullet points for multiple changes

### Footer (Optional)

- Include issue references: `Fixes: #123`
- Include breaking changes notices
- Include related pull requests: `Related to: #456`

## Commit Message Examples

### Good Examples

```
feat(cli-tools): Add recursive directory search to grep implementation

- Implement -r flag for recursive directory traversal
- Add proper error handling for inaccessible directories
- Update unit tests to cover recursive functionality
- Fix memory leak in pattern matching algorithm

Fixes: #45
```

```
fix(logger): Prevent race condition in multi-threaded logging

- Add mutex protection for shared log queue
- Implement proper RAII for lock management
- Add comprehensive thread safety tests

Related to: #67
```

```
docs: Update README with Docker setup instructions

- Add detailed Docker development workflow
- Include troubleshooting tips for common issues
- Update environment setup commands
```

```
test(memory-pool): Add performance benchmarks and stress tests

- Implement allocation/deallocation speed tests
- Add memory usage tracking utilities
- Create concurrent access test scenarios
- Verify thread safety under load
```

```
refactor(tcp-chat): Improve connection handling architecture

- Separate client management from message broadcasting
- Implement connection state tracking
- Add proper resource cleanup for disconnected clients
- Optimize message broadcast algorithm
```

### Bad Examples

```
Fixed some bugs
```

```
update code

fixed everything
```

```
changed stuff in the logger file because it was broken
```

## Commit Best Practices

### Single Concern Principle

Each commit should address a single logical change:

- Don't mix formatting changes with functional changes
- Don't fix multiple unrelated bugs in one commit
- Don't add features and update documentation in the same commit

### Atomic Commits

Make commits as small as possible while maintaining logical consistency:

```bash
# Good: Separate formatting from logic changes
git commit -m "style: format logger.cpp with clang-format"
git commit -m "feat: add thread id to log messages"

# Bad: Mixed changes
git commit -m "fix logging and format code"
```

### WIP Commits

Avoid WIP (Work In Progress) commits in the main branch:

- Use feature branches for ongoing work
- Squash WIP commits before merging
- Clean up commit history before pull requests

## Squashing and Rebasing

### Interactive Rebase

Use interactive rebase to clean up commit history:

```bash
# Rebase last 5 commits
git rebase -i HEAD~5

# Commands in rebase:
# pick: keep commit as-is
# squash: combine with previous commit
# edit: modify commit
# drop: remove commit
```

### Amending Commits

Fix commit messages or add to the latest commit:

```bash
# Fix the last commit message
git commit --amend -m "new commit message"

# Add more changes to the last commit
git add modified_file.cpp
git commit --amend --no-edit
```

## Branching Strategy

### Feature Branches

- Use descriptive branch names: `feature/user-authentication`
- Keep branches focused on a single feature or fix
- Branch from `main` and rebase regularly
- Remove branches after merging

### Pull Requests

- Keep pull requests focused on a single feature or issue
- Include a clear description of changes
- Reference related issues
- Ensure all tests pass before requesting review

## Signed-off-by and DCO

For compliance, sign your commits:

```bash
# Sign commits automatically
git config --local commit.gpgsign true

# Or manually sign with Signed-off-by
git commit -s -m "Your commit message"
```

## Common Commit Scenarios

### Adding a New Feature

```
feat(http-server): implement file upload functionality

- Add POST request handling
- Implement multipart form data parsing
- Add file validation and security checks
- Create upload progress tracking
- Update documentation with usage examples

Resolves: #123
```

### Fixing a Bug

```
fix(tcp-chat): prevent message corruption during concurrent sends

- Add proper synchronization for message queuing
- Implement atomic operations for shared counters
- Add null pointer checks for client connections
- Include regression tests for concurrent scenarios

Fixes: #145
```

### Refactoring Code

```
refactor(json-parser): improve error handling and validation

- Replace raw exception throwing with Result type
- Add detailed error context information
- Implement validation for all JSON value types
- Refactor recursive parsing into iterative approach

Performance: reduce parsing time by 15%
```

### Updating Documentation

```
docs: enhance CLI tools usage documentation

- Add detailed parameter explanations
- Include usage examples with common scenarios  
- Update troubleshooting section
- Add performance best practices
```

## Commit Validation

### Pre-commit Hooks

Consider using pre-commit hooks to ensure standards:

```bash
#!/bin/sh
# Example pre-commit hook to validate commit messages
commit_msg=$(cat .git/COMMIT_EDITMSG)
echo "$commit_msg" | grep -E "^(feat|fix|bug|docs|style|refactor|perf|test|build|ci|chore|revert)\(\w+\): .{1,72}$" >/dev/null

if [ $? -ne 0 ]; then
    echo "Error: Commit message doesn't follow format: type(scope): subject"
    exit 1
fi
```

### Checking Conventions

Before pushing, review your commits:

```bash
# Check last commit
git show --name-only

# View commit history
git log --oneline -10

# Check commit message format
git log --format="%s" -1
```

## Handling Mistakes

### Fixing Commit Messages

For commits not yet pushed:

```bash
# Fix last commit message
git commit --amend -m "properly formatted commit message"

# Fix older commit (rebase)
git rebase -i HEAD~3
```

### Changing Already Pushed Commits

If you must change already pushed commits:

```bash
# Amend and force push (use with caution!)
git commit --amend -m "new message"
git push --force-with-lease origin branch-name
```

⚠️ **Warning**: Only force push to your own feature branches, never to `main` or shared branches.

## Review Checklist

Before making commits, verify:

- [ ] Commit message follows the specified format
- [ ] Type is appropriate for the changes made
- [ ] Subject line is descriptive and under 72 characters
- [ ] Body (if present) explains the changes clearly
- [ ] Commit addresses a single logical change
- [ ] All tests pass before committing
- [ ] No sensitive information in commit
- [ ] Proper scope identified (if applicable)

## Next Steps

Apply these Git commit standards consistently in all your project contributions. Proper commit messages make the project history valuable for future developers and maintainers.