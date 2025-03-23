# Competitive Programming C++ Template

![GitHub repo size](https://img.shields.io/github/repo-size/Peeyush-04/CompetitiveEdge)
![GitHub last commit](https://img.shields.io/github/last-commit/Peeyush-04/CompetitiveEdge)

Welcome to the **Competitive Programming C++ Template**! This repository, maintained by [Peeyush-04](https://github.com/Peeyush-04), offers a modular and reusable C++ template designed to streamline your coding process for competitive programming. Whether you're debugging locally or submitting your solution to online judges like Codeforces or USACO, this template provides flexibility and efficiency for all your contest needs.

---

## Features

- **Conditional Debugging:**  
  Toggle local debugging to display performance metrics (execution time, memory usage) and compare output against expected results.

- **Automatic I/O Redirection:**  
  Seamlessly switch between file-based input/output for local testing and standard `cin`/`cout` for online submissions.

- **Minimal Design:**  
  A lightweight template optimized for quick coding and fast submissions on various online platforms.

- **USACO Support:**  
  Built-in compatibility for USACO-style file input/output, making local practice easier and more efficient.

---

## Directory Structure

- **`template.cpp`**  
  The main template file that includes all core functionalities for both local development and online submissions.

- **`debug.h`**  
  An optional header providing debugging utilities, enabled only when compiling with the local debug flag.

- **`test/`**  
  Contains:
  - `test.in` – Sample input file for local testing.
  - `test.out` – Output file generated during local execution.
  - `test-expected.out` – (Optional) Expected output for verifying correctness.

---

## How to Use

### Local Development

1. **Compile with Debugging Enabled:**
   ```bash
   g++ -DLOCAL_DEBUG template.cpp -o program
   ```
   Make sure that `debug.h` is located in the same directory as `template.cpp`.

2. **Prepare Your Test Files:**
   - Place your input data in `test/test.in`.
   - (Optional) Provide expected output in `test/test-expected.out` for result comparison.

3. **Run Your Program:**
   ```bash
   ./program
   ```
   Your output will be written to `test/test.out` and detailed debug information (like execution time and memory usage) will be displayed in the console.

### Online Judge Submission

1. **Prepare the Code:**
   - Open `template.cpp`.
   - Comment out or remove the `#include "debug.h"` line to disable local debugging features.
   - Implement your problem-specific solution inside the `solve()` function.

2. **Submit Your Code:**
   - Copy the modified `template.cpp` into your online judge’s submission interface.
   - The template uses standard `cin`/`cout` when local debugging is disabled.

---

## Customization

- **Problem-Specific Logic:**  
  Update the `solve()` function in `template.cpp` with your solution code for each contest problem.

- **Additional Headers:**  
  Include any extra libraries you need in the "Common Includes" section of `template.cpp`.

- **Global Variables:**  
  Define any necessary global variables above the `solve()` function.

---

## Contributing

Contributions are welcome! To get involved:
- Open an issue to report bugs or suggest improvements.
- Submit a pull request with your enhancements or fixes.

Visit the [Issues page](https://github.com/Peeyush-04/CompetitiveEdge/issues) to start contributing.

---

## License

This project is licensed under the [MIT License](LICENSE), allowing you to use, modify, and distribute it freely.

---

Happy coding and best of luck in your competitions!
