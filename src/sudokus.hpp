#ifndef SUDOKUS_SUDOKUS_HPP
#define SUDOKUS_SUDOKUS_HPP

#include <string>
#include <SFML/Graphics.hpp>

#include <iostream>

#include "utils.hpp"
#include "states/game-state.hpp"
#include "states/splashscreen-state.hpp"
#include "states/playing-state.hpp"
#include "states/solving-state.hpp"

class Sudokus
{
public:
    int SCREEN_WIDTH = 1500;
    int SCREEN_HEIGHT = 1500;

    Sudokus();
    void start();
    void gameLoop();

private:
    State _state;

    Grid _grid;

    std::map<State, GameState*> _stateInstances;
    sf::RenderWindow _window;
};

#endif // SUDOKUS_SUDOKUS_HPP