#ifndef SUDOKU_GRID_HPP
#define SUDOKU_GRID_HPP

#include <SFML/Graphics.hpp>
#include <fstream>

class Grid
{
public:
    Grid();
    ~Grid(){};
    void draw(sf::RenderWindow *window);

    int verifyRow(int i);
    int verifyColumn(int j);
    int verifyBlock(int i, int j);

    void loadGrid(std::string filename);

    void setRowColor(int i, sf::Color color);
    void setColumnColor(int j, sf::Color color);
    void setBlockColor(int i, int j, sf::Color color);

    int getValue(int i, int j);
    void setValue(int i, int j, int value);
    void resetValues();

    // void initMsgf();
private:
    // sf::Font _font;
    // sf::Text _cells[9][9];
    sf::RectangleShape _blocks[3][3];
    sf::RectangleShape _rows[9];
    sf::RectangleShape _columns[9];

    int _values[9][9] = {0};
};
#endif // SUDOKU_GRID_HPP