#include "playing-state.hpp"

void PlayingState::init(Grid *grid)
{
    _grid = grid;

    if (!_font.loadFromFile("../assets/fonts/iAWriterDuospace-Regular.otf"))
        std::cout << "Error while loading font" << std::endl;

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            _cells[i][j].setFont(_font);
            _cells[i][j].setFillColor(sf::Color::Black);
            _cells[i][j].setCharacterSize(150);
            _cells[i][j].setPosition(j*150 + 355, i*150 + 50);
        }
    }

    _selection.setSize(sf::Vector2f(150, 150));
    _selection.setPosition(sf::Vector2f(150*(_jSelected) + 325, 150 * (_iSelected) + 75));
    _selection.setFillColor(sf::Color::Transparent);
    _selection.setOutlineColor(sf::Color::Blue);
    _selection.setOutlineThickness(10);
}

void PlayingState::handleInput(sf::Event *event, State *state)
{
    if (event->type == sf::Event::KeyPressed)
    {
        if (event->key.code == sf::Keyboard::Return) *state = Solving;

        if (event->key.code == sf::Keyboard::Escape) _grid->resetValues();

        // Load grids from file for demonstration
        if (event->key.code == sf::Keyboard::E) _grid->loadGrid("../assets/grids/easy.txt");
        if (event->key.code == sf::Keyboard::M) _grid->loadGrid("../assets/grids/medium.txt");
        if (event->key.code == sf::Keyboard::H) _grid->loadGrid("../assets/grids/hard.txt");

        if (event->key.code == sf::Keyboard::Num1) _grid->setValue(_iSelected, _jSelected, 1);
        if (event->key.code == sf::Keyboard::Num2) _grid->setValue(_iSelected, _jSelected, 2);
        if (event->key.code == sf::Keyboard::Num3) _grid->setValue(_iSelected, _jSelected, 3);
        if (event->key.code == sf::Keyboard::Num4) _grid->setValue(_iSelected, _jSelected, 4);
        if (event->key.code == sf::Keyboard::Num5) _grid->setValue(_iSelected, _jSelected, 5);
        if (event->key.code == sf::Keyboard::Num6) _grid->setValue(_iSelected, _jSelected, 6);
        if (event->key.code == sf::Keyboard::Num7) _grid->setValue(_iSelected, _jSelected, 7);
        if (event->key.code == sf::Keyboard::Num8) _grid->setValue(_iSelected, _jSelected, 8);
        if (event->key.code == sf::Keyboard::Num9) _grid->setValue(_iSelected, _jSelected, 9);
        if (event->key.code == sf::Keyboard::Backspace) _grid->setValue(_iSelected, _jSelected, 0);

        if (event->key.code == sf::Keyboard::Left && _jSelected > 0) _jSelected--;
        if (event->key.code == sf::Keyboard::Right && _jSelected < 8) _jSelected++;
        if (event->key.code == sf::Keyboard::Down && _iSelected < 8) _iSelected++;
        if (event->key.code == sf::Keyboard::Up && _iSelected > 0) _iSelected--;
    }
}

void PlayingState::update(State *state)
{
    for (int i=0; i<9; i++)
    {
        for (int j=0; j<9; j++)
        {
            _cells[i][j].setString(std::to_string(_grid->getValue(i, j)));
        }
    }

    for (int i=0; i<9; i++)
    {
        if (_grid->verifyRow(i) == 0) _grid->setRowColor(i, sf::Color(255, 0, 0, 100));
        else _grid->setRowColor(i, sf::Color::Transparent);
    }

    for (int j=0; j<9; j++)
    {
        if (_grid->verifyColumn(j) == 0) _grid->setColumnColor(j, sf::Color(255, 0, 0, 100));
        else _grid->setColumnColor(j, sf::Color::Transparent);
    }

    for (int i=0; i<3; i++)
    {
        for (int j=0; j<3; j++)
        {
            if (_grid->verifyBlock(i, j) == 0) _grid->setBlockColor(i, j, sf::Color(255, 0, 0, 100));
            else _grid->setBlockColor(i, j, sf::Color::Transparent);
        }
    }
    _selection.setPosition(sf::Vector2f(150*(_jSelected) + 325, 150 * (_iSelected) + 75));
    
    // _visibleTextManager.updateAll();
}

void PlayingState::draw(sf::RenderWindow *window)
{
    _grid->draw(window);
    window->draw(_selection);

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (_grid->getValue(i, j) > 0)
            {
                window->draw(_cells[i][j]);
            }
        }
    }
    // _visibleTextManager.drawAll(window);
}