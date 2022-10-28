#ifndef SUDOKUS_SOLVING_STATE_HPP
#define SUDOKUS_SOLVING_STATE_HPP

#include <SFML/Graphics.hpp>

#include <iostream>
#include <bitset>
#include <bit>

#include "../utils.hpp"
#include "game-state.hpp"

#include "../grid.hpp"

class SolvingState : public GameState
{
public:
    void init(Grid *grid) override;
    void handleInput(sf::Event *event, State *state) override;
    void update(State *state) override;
    void draw(sf::RenderWindow *window) override;

    bool updateSolver();
    bool stopSolver();
    void computeBackwardMsg(std::bitset<9> *msg);
    bool isInSet(int val, std::bitset<9> set);
    bool isValid(int val, std::bitset<9> prevSet, std::bitset<9> set);
    std::string getString(int i, int j);
    int getRightMostSetBit(std::bitset<9> bits);

private:
    int _screenWidth, _screenHeight;
    Grid *_grid;

    bool _stepByStep;
    bool _solved;
    bool _error;


    std::bitset<9> _msgf[9][9];
    std::bitset<9> _msgbr[9][9];
    std::bitset<9> _msgbc[9][9];
    std::bitset<9> _msgbb[9][9];

    sf::Clock _clock;

    sf::Font _font;
    sf::Text _cells[9][9];

    // Intern states
    std::bitset<9> _sets[512] = {0};
    const int _numSets[10] = {1, 9, 36, 84, 126, 126, 84, 36, 9, 1};
    const int _startSets[10] = {0, 1, 10, 46, 130, 256, 382, 466, 502, 511};
};
#endif // SUDOKUS_SOLVING_STATE_HPP