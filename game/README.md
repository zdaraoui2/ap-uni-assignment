# Game System

This project is a simple game system that allows game components to update their position on the screen and perform actions over time.

## Project Structure

- **Game**: The core class that manages game components and handles the main game loop.

  - `Game.cpp`: Contains the game loop, adds components, and runs them at set intervals.
  - `Game.h`: Declares the `Game` class with methods to add components, run the game, and set initialization/termination callbacks.

- **GameComponent**: The base class for all components in the game.

  - `GameComponent.cpp`: Handles generic game component functionality, such as updating based on time.
  - `GameComponent.h`: Declares the `GameComponent` class and its attributes like `id` and the update method.

- **DrawableGameComponent**: A specialized game component that can move on the screen and change direction randomly.
  - `DrawableGameComponent.cpp`: Contains the logic for position updates, direction changes, and rendering the component.
  - `DrawableGameComponent.h`: Declares the `DrawableGameComponent` class and defines screen dimensions.

### `tests/`

- **Tests for Game Components**:
  - `GameTest.cpp`: Tests the game initialization, adding components, and running the game.
  - `DrawableGameComponentTest.cpp`: Tests initialization, updating position, and changing direction in `DrawableGameComponent`.
  - `GameComponentTest.cpp`: Tests basic functionality of `GameComponent`.

## How to Build and Run

To compile and run the game

```bash
c++ -std=c++11 -o Game main.cpp Game.cpp GameComponent.cpp DrawableGameComponent.cpp
./Game
```

To compile and run the tests, use the following commands:

```bash
# Compile the game component test
c++ -std=c++11 -o GameComponentTest game/tests/GameComponentTest.cpp game/GameComponent.cpp
./GameComponentTest

# Compile the drawable game component test
c++ -std=c++11 -o DrawableGameComponentTest game/tests/DrawableGameComponentTest.cpp game/DrawableGameComponent.cpp
./DrawableGameComponentTest

# Compile and run the full game test
c++ -std=c++11 -o GameTest game/tests/GameTest.cpp game/Game.cpp game/GameComponent.cpp game/DrawableGameComponent.cpp
./GameTest
```
