#include "grid.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <typeinfo>


Grid::Grid() 
{
    if (!_font.loadFromFile("../assets/fonts/cmunss.ttf"))
        cout << "Error while loading font" << endl;

    for (int i=0; i<9; i++)
    {
        for (int j=0; j<9; j++)
        {
            _cellsShape[i][j].setSize(sf::Vector2f(100, 100));
            _cellsShape[i][j].setFillColor(sf::Color::Transparent);
            _cellsShape[i][j].setOutlineColor(sf::Color::Black);
            _cellsShape[i][j].setOutlineThickness(3);
            _cellsShape[i][j].setOrigin(50, 50);
            _cellsShape[i][j].setPosition(j*100 + 350, i*100 + 350);

            _cellsText[i][j].setFont(_font);
            _cellsText[i][j].setCharacterSize(100);
        }
    }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            _blocksShape[i][j].setFillColor(sf::Color::Transparent);
            _blocksShape[i][j].setOutlineColor(sf::Color::Black);
            _blocksShape[i][j].setOutlineThickness(5);
            _blocksShape[i][j].setSize(sf::Vector2f(300, 300));
            _blocksShape[i][j].setOrigin(150, 150);
            _blocksShape[i][j].setPosition(sf::Vector2f(300 * j + 450, 300 * i + 450));
        }
    }

    _gridShape.setFillColor(sf::Color::Transparent);
    _gridShape.setOutlineColor(sf::Color::Black);
    _gridShape.setOutlineThickness(10);
    _gridShape.setSize(sf::Vector2f(900, 900));
    _gridShape.setOrigin(450, 450);
    _gridShape.setPosition(sf::Vector2f(750, 750));
}

void Grid::setCellColor(int i, int j, sf::Color color)
{
    _cellsShape[i][j].setFillColor(color);
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

void Grid::loadGrid(string filename)
{
    resetAllValues();

    ifstream infile(filename);

    if (!infile.is_open())
    {
        cout << "Error while loading grid" << endl;
        return;
    }

    char c;
    for (int i=0; i<9; i++)
    {
        for (int j=0; j<9; j++)
        {
            do
            {
                infile.get(c);
            } while (c == ' ' || c == '|' || c == '-' || c == '\n');
            
            if (c == '.') continue;
            _fixedValues[i][j] = (int) c - '0';
        }
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

void Grid::update(State *state)
{
    if (*state == Playing)
    {
        // draw fixed values
        for (int i=0; i<9; i++)
        {
            for (int j=0; j<9; j++)
            {
                _cellsShape[i][j].setFillColor(sf::Color::Transparent);
                if (_fixedValues[i][j] > 0) 
                {
                    _cellsText[i][j].setFillColor(sf::Color::Black);
                    _cellsText[i][j].setString(to_string(_fixedValues[i][j]));
                }
                else
                {
                    _cellsText[i][j].setFillColor(sf::Color::Black);
                    _cellsText[i][j].setString("");
                }
                sf::FloatRect cellTextRect = _cellsText[i][j].getLocalBounds();
                _cellsText[i][j].setOrigin(cellTextRect.left + cellTextRect.width/2, cellTextRect.top + cellTextRect.height/2);
                _cellsText[i][j].setPosition(_cellsShape[i][j].getPosition());
            }
        }
    }
    else if (*state == Solving)
    {
        for (int i=0; i<9; i++)
        {
            for (int j=0; j<9; j++)
            {
                _cellsShape[i][j].setFillColor(sf::Color::Transparent);
                if (_fixedValues[i][j] > 0) 
                {
                    _cellsText[i][j].setFillColor(sf::Color::Black);
                    _cellsText[i][j].setString(to_string(_fixedValues[i][j]));
                }
                else if (_solvedValues[i][j] > 0)
                {
                    _cellsText[i][j].setFillColor(sf::Color::Red);
                    _cellsText[i][j].setString(to_string(_solvedValues[i][j]));
                }
                sf::FloatRect cellTextRect = _cellsText[i][j].getLocalBounds();
                _cellsText[i][j].setOrigin(cellTextRect.left + cellTextRect.width/2, cellTextRect.top + cellTextRect.height/2);
            }
        }
    }
}

void Grid::draw(sf::RenderWindow *window)
{
    window->draw(_gridShape);

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            window->draw(_blocksShape[i][j]);
        }
    }

    for (int i=0; i<9; i++)
    {
        for (int j=0; j<9; j++)
        {
            window->draw(_cellsShape[i][j]);
            window->draw(_cellsText[i][j]);
        }
    }
}