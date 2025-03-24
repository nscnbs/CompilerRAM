# Imperative Language Compiler

This project implements a compiler for a simple imperative language. It was developed to showcase expertise in compiler design and language processing, making it a great addition to any portfolio.

## Overview

The compiler processes source code written in a basic imperative language by performing:
- **Lexical Analysis** using Flex
- **Syntax Analysis** using Bison
- **Semantic Analysis and Code Generation** in C++
- **Code Execution** on a simple virtual machine (a basic assembler-like environment)

## Project Structure

- `/src` – Contains the source code for Flex, Bison, and C++.
- `/test` – Includes example programs written in the target language.

## Tools and Versions

- **GNU Bison:** 3.8.2
- **Flex:** 2.6.4
- **GNU Make:** 4.3
- **C++ Standard:** C++17

## Key Files

- **README.md** – This file.
- **Makefile** – Build instructions.
- **main.cpp** – Entry point of the compiler.
- **lexer.l** – Flex source file for lexical analysis.
- **parser.y** – Bison source file for syntax analysis.
- **manager.cpp / manager.hpp** – Manages compilation phases.
- **conditions.cpp / conditions.hpp** – Handles conditional statements.
- **expressions.cpp / expressions.hpp** – Processes expressions.
- **instructions.cpp / instructions.hpp** – Deals with executable instructions.
- **procedure.hpp** – Procedure definitions.
- **types.hpp** – Type definitions.
- **value.hpp** – Value handling.
- **div.hpp, mult.hpp, mod.hpp** – Arithmetic operations.

## How to Use

1. **Build the Compiler:**

   Run the following command to compile the project:

   make

2. **Run the Compiler:**

    After a successful build, execute the compiler by specifying the input source file and the output file path for the virtual machine:

    ./kompilator input_file /maszyna-virtualna/output.mr

3. **Execute the Generated Code:**
    Finally, run the generated output file on the virtual machine:
    
    ./maszyna-virtualna output.mr