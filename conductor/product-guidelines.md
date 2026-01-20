# Product Guidelines

## Documentation & Tone
- **Style:** Technical and precise. All documentation (README, internal docs) should provide objective, clear descriptions of system internals, algorithms, and architectural decisions.
- **Audience Focus:** Written for technical reviewers and hiring managers who value clarity, correctness, and a deep understanding of POSIX systems.

## Coding Standards & Style
- **Naming Conventions:** Use highly descriptive variable and function names to ensure the code is self-documenting.
- **Commenting:** Follow a minimalist approach. Comments should be used sparingly and only when necessary to explain non-obvious logic or high-level architectural flows.
- **Consistency:** Maintain strict adherence to the project's chosen style guide (e.g., 42 Norminette) throughout the entire codebase.

## Memory Management & Safety
- **Allocation Responsibility:** Each module is strictly responsible for deallocating the memory it allocates.
- **Safety First:** Always perform null-checks after memory allocation and zero-initialize pointers after freeing to prevent dangling references.
- **Verification:** Ensure that every component is verifiable through memory analysis tools like Valgrind to maintain a leak-free environment.

## Project Structure & Presentation
- **Organization:** Maintain a clean and logical directory structure, clearly separating source code, headers, and tests.
- **Visual Aids:** Utilize architecture diagrams (e.g., Mermaid) to provide immediate context on how the different modules of the shell interact.
- **Validation:** Use a README-based "Stability & Coverage" section to present high-level statistics on test passes, stability, and robustness to visitors.
