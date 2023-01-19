#include "grid.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <typeinfo>


Grid::Grid() 
{
    if (!_font.loadFromFile("../assets/fonts/iAWriterDuospace-Regular.otf"))
        cout << "Error while loading font" << endl;

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

    for (int i=0; i<9; i++)
    {
        for (int j=0; j<9; j++)
        {
            _cells[i][j].setFont(_font);
            _cells[i][j].setCharacterSize(150);
            // _cells[i][j].setString(to_string(_grid->getValue(i, j)));
            _cells[i][j].setPosition(j*150 + 355, i*150 + 50);
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

void Grid::setFixedValue(int i, int j, int value)
{
    _fixedValues[i][j] = value;
}

void Grid::resetFixedValues()
{
    for (int i=0; i<9; i++)
    {
        for (int j=0; j<9; j++)
        {
            _fixedValues[i][j] = 0;
        }
    }
}

void Grid::resetSolvedValues()
{
    for (int i=0; i<9; i++)
    {
        for (int j=0; j<9; j++)
        {
            _solvedValues[i][j] = 0;
        }
    }
}

void Grid::resetAllValues()
{
    for (int i=0; i<9; i++)
    {
        for (int j=0; j<9; j++)
        {
            _fixedValues[i][j] = 0;
            _solvedValues[i][j] = 0;
        }
    }
}

void Grid::loadGrid(std::string filename)
{
    std::ifstream infile(filename);
    std::string line, val;

    int *ptr = &_fixedValues[0][0];

    resetAllValues();

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

void Grid::setSolvedValue(int i, int j, int value)
{
    _solvedValues[i][j] = value;
}

int Grid::getSolvedValue(int i, int j)
{
    return _solvedValues[i][j];
}

int Grid::getFixedValue(int i, int j)
{
    return _fixedValues[i][j];
}

int Grid::checkFixedRow(int i)
{
    for (int sj=0; sj<9; sj++)
    {
        if (_fixedValues[i][sj] == 0) continue;
        for (int tj=sj+1; tj<9; tj++)
        {
            if (_fixedValues[i][sj] == _fixedValues[i][tj]) return 0;
        }
    }
    return 1;
}

int Grid::checkFixedColumn(int j)
{
    for (int i=0; i<9; i++)
    {
        if (_fixedValues[i][j] == 0) continue;
        for (int k=i+1; k<9; k++)
        {
            if (_fixedValues[i][j] == _fixedValues[k][j]) return 0;
        }
    }
    return 1;
}

int Grid::checkFixedBlock(int i, int j)
{
    for (int k=i*3; k<i*3+3; k++)
    {
        for (int l=j*3; l<j*3+3; l++)
        {
            if (_fixedValues[k][l] == 0) continue;
            for (int m=i*3; m<i*3+3; m++)
            {
                for (int n=j*3; n<j*3+3; n++)
                {
                    if (k*9+l <= m*9+n) continue;
                    if (_fixedValues[k][l] == _fixedValues[m][n]) return 0;
                }
            }
        }
    }
    return 1;
}

void Grid::update(State *state)
{
    if (*state == Playing)
    {
        // draw fixed values
        for (int i=0; i<9; i++)
        {
            for (int j=0; j<9; j++)
            {
                if (_fixedValues[i][j] > 0) 
                {
                    _cells[i][j].setFillColor(sf::Color::Black);
                    _cells[i][j].setString(to_string(_fixedValues[i][j]));
                }
                else
                {
                    _cells[i][j].setFillColor(sf::Color::Black);
                    _cells[i][j].setString("");
                }
            }
        }

        // check rows and color accordingly
        for (int i=0; i<9; i++)
        {
            if (checkFixedRow(i)) _rows[i].setFillColor(sf::Color::Transparent);
            else _rows[i].setFillColor(sf::Color(255, 0, 0, 100));
        }

        // check columns and color accordingly
        for (int j=0; j<9; j++)
        {
            if (checkFixedColumn(j)) _columns[j].setFillColor(sf::Color::Transparent);
            else _columns[j].setFillColor(sf::Color(255, 0, 0, 100));
        }

        // check blocks and color accordingly
        for (int i=0; i<3; i++)
        {
            for (int j=0; j<3; j++)
            {
                if (checkFixedBlock(i, j)) _blocks[i][j].setFillColor(sf::Color::Transparent);
                else _blocks[i][j].setFillColor(sf::Color(255, 0, 0, 100));
            }
        }
    }
    else if (*state == Solving)
    {
        for (int i=0; i<9; i++)
        {
            for (int j=0; j<9; j++)
            {
                if (_fixedValues[i][j] > 0) 
                {
                    _cells[i][j].setFillColor(sf::Color::Black);
                    _cells[i][j].setString(to_string(_fixedValues[i][j]));
                }
                else if (_solvedValues[i][j] > 0)
                {
                    _cells[i][j].setFillColor(sf::Color::Red);
                    _cells[i][j].setString(to_string(_solvedValues[i][j]));
                }
            }
        }
    }
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

    for (int i=0; i<9; i++)
    {
        for (int j=0; j<9; j++)
        {
            window->draw(_cells[i][j]);
        }
    }
}