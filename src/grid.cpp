#include "grid.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <typeinfo>


Grid::Grid() 
{
    if (!_font.loadFromFile("../assets/fonts/cmunss.ttf"))
        cout << "Error while loading font" << endl;

    _gridShape.setFillColor(sf::Color::Transparent);
    _gridShape.setOutlineColor(sf::Color::Black);
    _gridShape.setOutlineThickness(10);
    _gridShape.setSize(sf::Vector2f(9*_cellSize, 9*_cellSize));

    _view = sf::View(sf::FloatRect(-_cellSize, -_cellSize, 11*_cellSize, 11*_cellSize));

    for (int i=0; i<9; i++)
    {
        for (int j=0; j<9; j++)
        {
            _cellsShape[i][j].setSize(sf::Vector2f(_cellSize, _cellSize));
            _cellsShape[i][j].setFillColor(sf::Color::Transparent);
            _cellsShape[i][j].setOutlineColor(sf::Color::Black);
            _cellsShape[i][j].setOutlineThickness(2);
            _cellsShape[i][j].setOrigin(_cellSize/2, _cellSize/2);
            _cellsShape[i][j].setPosition((j+0.5)*_cellSize, (i+0.5)*_cellSize);

            _cellsText[i][j].setFont(_font);
            _cellsText[i][j].setCharacterSize(_cellSize);
        }
    }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            _blocksShape[i][j].setFillColor(sf::Color::Transparent);
            _blocksShape[i][j].setOutlineColor(sf::Color::Black);
            _blocksShape[i][j].setOutlineThickness(5);
            _blocksShape[i][j].setSize(sf::Vector2f(3*_cellSize, 3*_cellSize));
            _blocksShape[i][j].setOrigin((3*_cellSize)/2, (3*_cellSize)/2);
            _blocksShape[i][j].setPosition(sf::Vector2f((3*j+1.5)*_cellSize, (3*i+1.5)*_cellSize));
        }
    }
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

void Grid::loadEasyGrid()
{
    resetAllValues();

    int tmp[9][9] = {
        {0,5,0,0,0,0,0,7,0},
        {7,0,6,0,1,0,0,2,9},
        {9,0,0,0,0,4,0,6,0},
        {0,2,0,3,0,0,0,0,0},
        {0,1,0,0,6,0,0,0,0},
        {5,0,0,0,0,7,0,1,3},
        {0,0,0,9,4,0,0,0,0},
        {3,0,9,0,2,8,7,0,5},
        {0,4,0,5,7,0,0,0,6},
        };

    for (int i=0; i<9; i++)
    {
        for (int j=0; j<9; j++)
        {
            _fixedValues[i][j] = tmp[i][j];
        }
    }
}

void Grid::loadMultiSolutionsGrid()
{
    resetAllValues();
    
    int tmp[9][9] = {
        {0,0,0,2,0,0,8,0,0},
        {1,0,0,9,0,0,0,0,0},
        {0,3,0,0,7,0,6,0,0},
        {4,0,0,0,0,6,1,0,0},
        {3,0,0,7,0,0,0,9,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,4,0,2,0,0,1,0},
        {7,0,0,0,0,0,0,5,0},
        {0,2,8,0,0,0,0,0,0},
        };

    for (int i=0; i<9; i++)
    {
        for (int j=0; j<9; j++)
        {
            _fixedValues[i][j] = tmp[i][j];
        }
    }
}

void Grid::loadHardGrid()
{
    resetAllValues();
    
    int tmp[9][9] = {
        {8,0,0,0,0,0,0,0,0},
        {0,0,3,6,0,0,0,0,0},
        {0,7,0,0,9,0,2,0,0},
        {0,5,0,0,0,7,0,0,0},
        {0,0,0,0,4,5,7,0,0},
        {0,0,0,1,0,0,0,3,0},
        {0,0,1,0,0,0,0,6,8},
        {0,0,8,5,0,0,0,1,0},
        {0,9,0,0,0,0,4,0,0},
        };

    for (int i=0; i<9; i++)
    {
        for (int j=0; j<9; j++)
        {
            _fixedValues[i][j] = tmp[i][j];
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

    window->setView(_view);
}