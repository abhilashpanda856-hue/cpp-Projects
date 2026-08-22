# 🔌 Interactive Digital Logic Circuit Simulator

Welcome to my C++ Logic Circuit Simulator! This repository isn't just a final polished product; it is a documentation of my journey in learning advanced Object-Oriented C++, custom operator overloading, and graphics programming using SFML 3.0. 

Instead of trying to make it perfect from day one, I built this iteratively, solving architectural and environmental challenges as they appeared.

## 🚀 The Development Journey

**Phase 1: Conquering the SFML 3.0 Environment**
Before writing the logic, I had to overcome the hurdles of C++ package management. 
* I utilized MSYS2 to install the modern UCRT64 GCC compiler alongside SFML 3.0[cite: 3].
* I successfully debugged compiler pathing issues in `tasks.json` and resolved silent "Entry Point Not Found" DLL conflicts to get the graphical canvas rendering flawlessly[cite: 3].

**Phase 2: The Core OOP Architecture**
I wanted the physical concept of "wiring gates together" to feel natural in the code.
* **Abstract Base Classes:** I created a `LogicGate` base class with a pure virtual `evaluate()` function.
* **Operator Overloading:** I overloaded the `>>` operator so I could physically wire outputs to inputs directly in C++ (e.g., `switch1 >> andGate`).

**Phase 3: Error Handling & The Sandbox**
Finally, I made the system robust and modular for testing.
* **Custom Exceptions:** I implemented a `DisconnectedPinException`. If a gate is evaluated without being fully wired, the program catches the exception and visually turns the gate yellow to alert the user.
* **Polymorphic Sandbox:** I structured the `main()` loop as a commented sandbox utilizing a `LogicGate* activeGate` pointer. This polymorphism allows rapid swapping and testing of AND, OR, NOT, XOR, and NAND gates without rewriting the core rendering logic.

---

## 🛠️ How to Build and Run

1. Ensure you have the MSYS2 UCRT64 toolchain and SFML 3.0 installed[cite: 3].
2. Clone this repository to your local machine.
3. Open your project in VS Code and ensure your `.vscode/tasks.json` includes the `-lsfml-graphics`, `-lsfml-window`, and `-lsfml-system` linker flags[cite: 3].
4. Build the active file (Ctrl + Shift + B).
5. Run the executable in your terminal: `.\main.exe`

Enjoy wiring up your custom circuits!
