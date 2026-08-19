# 🗄️ Interactive In-Memory Relational Database Engine

Welcome to my C++ Database Engine project! This repository isn't just a final polished product; it's a documentation of my journey in learning advanced C++ memory management, type safety, and dynamic console formatting. 

Instead of trying to make it perfect from day one, I built this iteratively, solving problems as they appeared.

## 🚀 The Development Journey

### Phase 1: The Rough Draft
I started by building the core architectural skeleton. The goal was simple: take user input dynamically and show the output. I used:
* **C++ Templates:** To allow the database columns to store mixed data types (like `std::string`, `double`, and `long long`) seamlessly.
* **Raw Dynamic Memory:** Using custom constructors and destructors to allocate exact heap memory for new rows, avoiding memory leaks when the program ends.

### Phase 2: The Symmetrical Problem
Once the engine could take interactive inputs, I noticed a visual asymmetry: the console table looked jagged when printing names of different lengths. 
* **The Fix:** I upgraded the rendering logic by connecting the margin length to the longest data entry in each column. The engine now scans the entire table, calculates the string lengths using `<sstream>`, and dynamically adjusts the exact padding for perfectly straight margins.
* **Handling Spaces:** I also swapped standard `std::cin` for `std::getline` so the engine could correctly accept multi-word inputs (like full names with spaces) without breaking the loop.

### Phase 3: The 10-Digit Validation Rule
Finally, I needed to make the system robust for real-world student data. 
* **Data Limits:** A standard 32-bit `int` would overflow if given a massive Registration Number, so I upgraded the architecture to natively handle 64-bit `long long` integers.
* **Strict Validation:** I implemented a strict `while` loop that converts the input to a string, checks its length, and actively blocks any Registration Number that is not exactly 10 digits.

---

## 🛠️ How to Build and Run

1. Ensure you have a modern C++ compiler installed (C++11 or higher).
2. Clone this repository to your local machine.
3. Open your terminal in the project folder and compile the code:
   `g++ main.cpp -o db_engine.exe`
4. Run the executable:
   `.\db_engine.exe`

Enjoy interacting with the database!
