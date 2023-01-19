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




    // void get_all_configs(vector<vector<u_int16_t>> all_bin_configs, vector<u_int8_t> config, int idx);
    // int checkMsg(bitset<9> **msgf);

    // void initForwardMsgRow(bitset<9> *msgf, bitset<9> *msgfr, int idx);
    // void initForwardMsgColumn(bitset<9> *msgf, bitset<9> *msgfc, int idx);
    // void initForwardMsgBlock(bitset<9> *msgf, bitset<9> *msgfb, int idx);
    // void updateBackwardMsg();
    // int checkStoppingCriterion(bitset<9> *msfg);



    // bool isInSet(int val, bitset<9> set);
    // bool isValid(int val, bitset<9> prevSet, bitset<9> set);
    // string getString(int i, int j);
    // int getRightMostSetBit(bitset<9> bits);

private:
    int _screenWidth, _screenHeight;

    Grid *_grid;

    // bitset<9> _msgf[9][9];
    Solution* _solution = NULL;

    sf::Clock _clock;

    // // Intern states
    // const int _numSets[10] = {1, 9, 36, 84, 126, 126, 84, 36, 9, 1};
    // const int _startSets[10] = {0, 1, 10, 46, 130, 256, 382, 466, 502, 511};
};
#endif // SUDOKUS_SOLVING_STATE_HPP