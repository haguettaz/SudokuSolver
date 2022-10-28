#include "grid.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <typeinfo>


Grid::Grid() 
{
    for (int i=0; i<9; i++)
    {
        _rows[i].setSize(sf::Vector2f(1350, 150));
        _rows[i].setPosition(sf::Vector2f(325, 75 + 150 * i));
        _rows[i].setFillColor(sf::Color::Transparent);
        _rows[i].setOutlineColor(sf::Color::Black);
        _rows[i].setOutlineThickness(3);
    }

    for (int j=0; j<9; j++)
    {
        _columns[j].setSize(sf::Vector2f(150, 1350));
        _columns[j].setPosition(sf::Vector2f(325 + 150*j, 75));
        _columns[j].setFillColor(sf::Color::Transparent);
        _columns[j].setOutlineColor(sf::Color::Black);
        _columns[j].setOutlineThickness(3);
    }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            _blocks[i][j].setSize(sf::Vector2f(450, 450));
            _blocks[i][j].setPosition(sf::Vector2f(450 * j + 325, 450 * i + 75));
            _blocks[i][j].setFillColor(sf::Color::Transparent);
            _blocks[i][j].setOutlineColor(sf::Color::Black);
            _blocks[i][j].setOutlineThickness(5);
        }
    }
}

void Grid::setRowColor(int i, sf::Color color)
{
    _rows[i].setFillColor(color);
}

void Grid::setColumnColor(int j, sf::Color color)
{
    _columns[j].setFillColor(color);
}

void Grid::setBlockColor(int i, int j, sf::Color color)
{
    _blocks[i][j].setFillColor(color);
}

void Grid::resetValues()
{
    for (int i=0; i<9; i++)
    {
        for (int j=0; j<9; j++)
        {
            _values[i][j] = 0;
        }
    }
}
void Grid::loadGrid(std::string filename)
{
    std::ifstream infile(filename);
    std::string line, val;

    int *ptr = &_values[0][0];

    resetValues();

    while (std::getline(infile, line))
    {
        for (std::string::iterator it = line.begin(); it!=line.end(); ++it)
        {
            if (*it == ',') ptr++;
            else if (std::isdigit(*it)) *ptr = (int) *it - '0';
        }
        ptr++;
    }
}


void Grid::setValue(int i, int j, int value)
{
    _values[i][j] = value;
}

int Grid::getValue(int i, int j)
{
    return _values[i][j];
}

int Grid::verifyColumn(int j)
{
    for (int si=0; si<9; si++)
    {
        if (_values[si][j] == 0) continue;
        for (int ti=si+1; ti<9; ti++)
        {
            if (_values[si][j] == _values[ti][j]) return 0;
        }
    }
    return 1;
}

int Grid::verifyRow(int i)
{
    for (int sj=0; sj<9; sj++)
    {
        if (_values[i][sj] == 0) continue;
        for (int tj=sj+1; tj<9; tj++)
        {
            if (_values[i][sj] == _values[i][tj]) return 0;
        }
    }
    return 1;
}

int Grid::verifyBlock(int i, int j)
{
    for (int si=i*3; si<i*3+3; si++)
    {
        for (int sj=j*3; sj<j*3+3; sj++)
        {
            if (_values[si][sj] == 0) continue;
            for (int ti=i*3; ti<i*3+3; ti++)
            {
                for (int tj=j*3; tj<j*3+3; tj++)
                {
                    if (si*9+sj <= ti*9+tj) continue;
                    if (_values[si][sj] == _values[ti][tj]) return 0;
                }
            }
        }
    }
    return 1;
}

void Grid::draw(sf::RenderWindow *window)
{
    for (int i=0; i<9; i++)
    {
        window->draw(_rows[i]);
        window->draw(_columns[i]);
    }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            window->draw(_blocks[i][j]);
        }
    }
}