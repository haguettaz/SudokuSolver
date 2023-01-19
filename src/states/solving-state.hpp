#ifndef SUDOKUS_SOLVING_STATE_HPP
#define SUDOKUS_SOLVING_STATE_HPP

#include <SFML/Graphics.hpp>

#include <iostream>
#include <bitset>
#include <algorithm>
#include <vector>


#include "../utils.hpp"
#include "game-state.hpp"

#include "../grid.hpp"

using namespace std;

struct Solution
{
    int values[9][9];
    struct Solution *next;
    struct Solution *prev;
};

class SolvingState : public GameState
{
public:

    void init(Grid *grid) override;
    void handleInput(sf::Event *event, State *state) override;
    void update(State *state) override;
    void draw(sf::RenderWindow *window) override;

    bool updateSolver();
    void computeBackwardMsg(bitset<9> *msg);
    void computeForwardMsg(bitset<9> *msg);


    void solve(bitset<9> (&msgf)[9][9]);
    int iterativeConstraintPropagation(bitset<9> (&msgf)[9][9]);
    void updateBackwardMsg(bitset<9> (&msgf)[9][9], bitset<9> (&msgbr)[9][9], bitset<9> (&msgbc)[9][9], bitset<9> (&msgbb)[9][9]);
    int updateForwardMsg(bitset<9> (&msgf)[9][9], bitset<9> (&msgbr)[9][9], bitset<9> (&msgbc)[9][9], bitset<9> (&msgbb)[9][9]);
    void maxBackwardMsgRow(bitset<9> config, bitset<9> (&msgf)[9][9], int i, int j, int k, int *valid);
    void maxBackwardMsgColumn(bitset<9> config, bitset<9> (&msgf)[9][9], int i, int j, int k, int *valid);
    void maxBackwardMsgBlock(bitset<9> config, bitset<9> (&msgf)[9][9], int i, int j, int m, int n, int *valid);

    void addSolution(bitset<9> (&msgf)[9][9]);
    void cleanSolution();
    int getNbSolutions();

private:
    int _screenWidth, _screenHeight;

    Grid *_grid;

    Solution* _solution = NULL;

    sf::Clock _clock;
};
#endif // SUDOKUS_SOLVING_STATE_HPP