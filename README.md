# Snake

A small console-based Snake game written in C++. The player steers a growing snake around a rectangular board, eats food to earn points, and tries to avoid running into the snake's own body.

## Contents

- [Requirements](#requirements)
- [Build and run](#build-and-run)
- [How to play](#how-to-play)
- [Game rules and behavior](#game-rules-and-behavior)
- [Code structure](#code-structure)
- [Object-oriented design](#object-oriented-design)
- [C++ concepts practiced](#c-concepts-practiced)
- [Current limitations](#current-limitations)

## Requirements

- Windows
- A C++ compiler such as MinGW-w64 `g++`
- A console terminal that supports ANSI escape sequences

The program uses `<conio.h>` functions (`_kbhit` and `_getch`) for non-blocking keyboard input. It also uses ANSI escape sequences to reposition the cursor and hide it while playing, so behavior may vary in terminals that do not support those sequences.

## Build and run

Open PowerShell or another terminal in the project folder and compile with MinGW-w64:

```powershell
g++ -std=c++11 main.cpp -o snake.exe
```

Start the game in PowerShell:

```powershell
.\snake.exe
```

The project currently consists of a single source file, `main.cpp`. The source includes `<bits/stdc++.h>`, a GCC convenience header; a compiler that does not provide it may require replacing it with individual standard-library headers.

## How to play

- Press `W` to move up.
- Press `A` to move left.
- Press `S` to move down.
- Press `D` to move right.
- Eat the food, shown as `@`, to score a point and grow.
- Avoid running into your own body, shown as `*`.

Input is polled while the game runs, so the snake continues moving between key presses. The game ends when the snake collides with itself. There is no separate quit key; close the console to exit early.

## Game rules and behavior

- The board is 35 columns wide and 20 rows tall, including its border.
- `#` marks the border, `*` marks the snake, and `@` marks the food.
- The snake starts with three segments and moves one grid position per game tick.
- The snake wraps to the opposite side when it crosses a board edge; hitting a wall does not end the game.
- The snake cannot immediately reverse direction, preventing it from turning directly back into itself.
- Food is randomly placed inside the border and regenerated if its position overlaps the snake.
- Eating food increments the score and adds a segment to the snake.
- Movement accelerates as the score rises:

| Score | Delay per move |
| --- | ---: |
| 0–7 | 150 ms |
| 8–17 | 120 ms |
| 18–24 | 100 ms |
| 25 and above | 80 ms |

## Code structure

The program keeps the game in one file and separates several responsibilities into small types and functions:

- `position` stores an `x` and `y` coordinate.
- `Snake` owns the body and current direction, and implements movement, growth, direction changes, and self-collision detection.
- `Food` owns a position and generates random positions for new food.
- `renderWorld` draws the board, snake, and food.
- `renderScore` displays the current score.
- `getDelay` selects the movement interval based on the score.
- `foodInSnake` checks whether a generated food position is occupied.
- `main` creates the game objects and runs the input, update, collision, scoring, and rendering loop.

Each game-loop iteration checks for input, moves the snake, checks for self-collision, handles food, redraws the board, and waits for the selected delay.

## Object-oriented design

The design groups state with the operations that act on it:

- A `Snake` object keeps its body and direction as private data. Other parts of the program interact with it through public methods such as `move`, `grow`, `changeDirection`, and `hasCollided`.
- A `Food` object keeps its position and provides methods to read it and generate another position.
- The `main` function owns one `Snake` and one `Food` object and coordinates their interaction. Rendering functions receive these objects by `const` reference, allowing them to inspect game state without copying or modifying it.

This is an example of encapsulation and separation of responsibilities. The game uses objects and classes, but does not use inheritance or runtime polymorphism.

## C++ concepts practiced

- Defining a `struct` and classes, creating objects, and writing constructors.
- Encapsulation through private data and public member functions.
- Standard containers: `vector` for the snake body and `unordered_map` for direction lookup.
- References and `const` correctness, including read-only access to object state.
- Functions, parameters, return values, loops, conditionals, and Boolean collision checks.
- Random number generation with `random_device`, `mt19937`, and `uniform_int_distribution`.
- Keyboard polling with `_kbhit` and `_getch`.
- Timing with `chrono` and `this_thread::sleep_for`.
- Breaking a program into input, update, and render steps inside a game loop.

## Current limitations

- The game is designed for a Windows console and uses `<conio.h>` for input.
- The food and snake are rendered as text characters; there is no graphical interface or sound.
- The score is shown only during the current run; there is no saved high score or restart menu.
- The snake can pass through the outer border because the board wraps at its edges.