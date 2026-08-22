# 🧠 ML Tokenizer Engine (C++)

Welcome to the **ML Tokenizer Engine**! This project is a modern, object-oriented C++ implementation of a text tokenization system, fundamental to how Machine Learning and AI models process natural language.

Instead of keeping this as a simple C-style array script, I evolved it into a robust C++ class architecture, demonstrating encapsulation, dynamic memory management, and custom hashing.

## 🚀 The Development Journey

**Phase 1: The C-Style Foundation**
I initially built this engine in C for my Data Structures innovative assignment. It utilized:
* A `Node` struct to form a Singly Linked List for collision resolution.
* A raw array of pointers (`Node* hashTable[]`) to act as the hash map.
* The highly efficient `djb2` hash function to distribute string keys evenly.

**Phase 2: The Modern C++ OOP Upgrade**
To make the engine production-ready and prevent memory leaks, I completely refactored the architecture into C++:
* **Encapsulation:** I wrapped the entire data structure inside a `Tokenizer` class, hiding the internal `Node` struct and hash table array from the public interface.
* **Dynamic Destructors:** I replaced manual `free()` calls with a rigorous `~Tokenizer()` destructor that automatically traverses every chain in the hash table, safely deleting all nodes when the object goes out of scope.
* **Modern Standard Library:** I swapped raw `char*` pointers and `strtok` for `std::string` and `std::stringstream`, making the text parsing significantly safer and easier to maintain.

## 🧠 Core Features & Algorithms
* **djb2 Hashing:** Utilizes the fast bitwise hashing algorithm (`hash * 33 + c`) to map strings to unique integer indices.
* **Chaining Collision Resolution:** If two different words map to the same hash index, they are safely stored in a dynamically allocated Singly Linked List.
* **Dynamic Vocabulary Mapping:** Assigns a unique integer `token_id` to every new word it encounters, mimicking the exact first step of an AI Natural Language Processing pipeline.

---

## 🛠️ How to Build and Run

1. Ensure you have a modern C++ compiler installed (C++11 or higher).
2. Clone this repository to your local machine.
3. Open your terminal in the `ML_Tokenizer_Engine` folder and compile the code:
   ```bash
   g++ main.cpp -o tokenizer.exe
