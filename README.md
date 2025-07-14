# Angry Politicians

This project is a complete implementation of a 2D game called "Angry Politicians," inspired by the mechanics of "Angry Birds." It was developed in C++ using the **SFML** library for rendering and window management. Its most notable technical feature is a hybrid physics engine, which consists of a custom wrapper around **Box2D** to achieve precise and flexible collision detection using the SAT (Separating Axis Theorem) algorithm.

## Requirements

* **Linux** Operating System (why? cos we believe in open source, fuck off microsoft).
* **g++ >= 13** C++ compiler.
* **SFML >= 2.6** library.
* `Makefile` build automation tool.

## Installation and Compilation

1.  **Verify g++ and Install SFML:**
    Open a terminal. First, check your g++ version:
    ```bash
    g++ --version
    ```
    If you do not meet the version requirement, please update your compiler. Next, install the SFML development library.
    ```bash
    sudo apt-get update
    sudo apt-get install libsfml-dev
    ```

2.  **Clone and Compile the Project:**
    Navigate to the folder where you cloned the project repository and use the `make` command to compile it.
    ```bash
    cd PROJECT-CS4016/
    make
    ```

3.  **Run:**
    Once compiled, an executable named `angry_politicians` will be generated. Launch it from the terminal to play.
    ```bash
    ./game
    ```
