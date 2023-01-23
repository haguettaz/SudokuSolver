#ifndef SUDOKU_GRID_HPP
#define SUDOKU_GRID_HPP

#include <SFML/Graphics.hpp>
#include <fstream>

#include "utils.hpp"

using namespace std;
class Grid
{
public:
    Grid();
    ~Grid(){};
    void update(State *state);
    void draw(sf::RenderWindow *window);

    void loadEasyGrid();
    void loadMultiSolutionsGrid();
    void loadHardGrid();

    void setCellColor(int i, int j, sf::Color color);

    int getFixedValue(int i, int j);
    int getSolvedValue(int i, int j);

    void setFixedValue(int i, int j, int value);
    void setSolvedValue(int i, int j, int value);

    void resetFixedValues();
    void resetSolvedValues();
    void resetAllValues();

private:
    int _cellSize = 100;

    sf::View _view;
    sf::Font _font;
    sf::Text _cellsText[9][9];
    sf::RectangleShape _cellsShape[9][9];
    sf::RectangleShape _blocksShape[3][3];
    sf::RectangleShape _gridShape;

    int _fixedValues[9][9] = {0};
    int _solvedValues[9][9] = {0};
};
#endif // SUDOKU_GRID_HPP