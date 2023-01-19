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

    int checkFixedRow(int i);
    int checkFixedColumn(int j);
    int checkFixedBlock(int i, int j);

    void loadGrid(std::string filename);

    void setRowColor(int i, sf::Color color);
    void setColumnColor(int j, sf::Color color);
    void setBlockColor(int i, int j, sf::Color color);

    int getFixedValue(int i, int j);
    int getSolvedValue(int i, int j);

    void setFixedValue(int i, int j, int value);
    void setSolvedValue(int i, int j, int value);

    void resetFixedValues();
    void resetSolvedValues();
    void resetAllValues();

private:
    sf::Font _font;

    sf::Text _cells[9][9];
    sf::RectangleShape _blocks[3][3];
    sf::RectangleShape _rows[9];
    sf::RectangleShape _columns[9];

    int _fixedValues[9][9] = {0};
    int _solvedValues[9][9] = {0};
};
#endif // SUDOKU_GRID_HPP