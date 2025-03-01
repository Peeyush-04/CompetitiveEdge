# Competitive Programming C++ Template

![GitHub repo size](https://img.shields.io/github/repo-size/Peeyush-04/CompetitiveEdge)
![GitHub last commit](https://img.shields.io/github/last-commit/Peeyush-04/CompetitiveEdge)

Welcome to the **Competitive Programming C++ Template**! This repository, maintained by [Peeyush-04](https://github.com/Peeyush-04), provides a modular and reusable C++ template designed for competitive programming. Whether you're debugging locally or submitting to online judges like Codeforces or USACO, this template offers flexibility and efficiency to streamline your coding process.

---

## Features

- **Conditional Debugging**: Provides performance metrics (time, memory) and output comparison for local testing when enabled.
- **Automatic I/O Redirection**: Switches between file-based I/O for local testing and standard `cin`/`cout` for online judges.
- **Minimal Design**: Lightweight and optimized for quick submissions to online platforms.
- **USACO Support**: Compatible with USACO-style file input/output for local practice.

---

## Directory Structure

- **`template.cpp`**: The core template file for both local development and online submissions.
- **`debug.h`**: Optional debugging utilities, activated only for local testing with a specific compilation flag.
- **`test/`**: Folder containing:
  - `test.in`: Input file for local testing.
  - `test.out`: Output file generated during local runs.
  - `test-expected.out`: Expected output file for comparison during debugging (optional).

---

## How to Use

### Local Development
1. **Compile with Debugging**:
   ```bash
   g++ -DLOCAL_DEBUG template.cpp -o program
   ```
   - Ensure `debug.h` is in the same directory as `template.cpp`.
2. **Set Up Test Files**:
   - Place input data in `test/test.in`.
   - Optionally, add expected output to `test/test-expected.out` for validation.
3. **Run the Program**:
   ```bash
   ./program
   ```
   - Output is saved to `test/test.out`.
   - Debug information (execution time, memory usage, and output comparison) is printed to the console.

### Online Judge Submission
1. **Prepare the Code**:
   - Open `template.cpp`.
   - Comment out or remove the `#include "debug.h"` line.
   - Modify the `solve()` function to implement your problem-specific logic.
2. **Submit**:
   - Copy-paste the modified `template.cpp` into the online judge’s submission interface.
   - The template uses `cin`/`cout` by default when no local files are detected.

---

## Customization

- **Problem-Specific Logic**: Update the `solve()` function in `template.cpp` with your solution code.
- **Additional Headers**: Add required `#include` directives to the "Common Includes" section of `template.cpp`.
- **Global Variables**: Define any necessary globals above the `solve()` function in `template.cpp`.

---

## Contributing

We welcome contributions to enhance this template! To get involved:
- Open an issue to report bugs or suggest features.
- Submit a pull request with your improvements.

Visit the [Issues page](https://github.com/Peeyush-04/CompetitiveEdge/issues) to start contributing.

---

## License

This project is licensed under the [MIT License](LICENSE), allowing you to freely use, modify, and distribute it as needed.
