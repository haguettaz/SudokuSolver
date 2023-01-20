#include "playing-state.hpp"

void PlayingState::init(Grid *grid)
{
    _grid = grid;
}

void PlayingState::handleInput(sf::Event *event, State *state)
{
    if (event->type == sf::Event::KeyPressed)
    {
        if (event->key.code == sf::Keyboard::Return) 
        {
            *state = Solving;
            _grid->resetSolvedValues();
        }

        if (event->key.code == sf::Keyboard::Escape) _grid->resetAllValues();

        if (event->key.code == sf::Keyboard::E) _grid->loadGrid("../assets/grids/easy.txt");
        if (event->key.code == sf::Keyboard::M) _grid->loadGrid("../assets/grids/multiple.txt");
        if (event->key.code == sf::Keyboard::H) _grid->loadGrid("../assets/grids/hard.txt");

        if (event->key.code == sf::Keyboard::Num1) _grid->setFixedValue(_iSelected, _jSelected, 1);
        if (event->key.code == sf::Keyboard::Num2) _grid->setFixedValue(_iSelected, _jSelected, 2);
        if (event->key.code == sf::Keyboard::Num3) _grid->setFixedValue(_iSelected, _jSelected, 3);
        if (event->key.code == sf::Keyboard::Num4) _grid->setFixedValue(_iSelected, _jSelected, 4);
        if (event->key.code == sf::Keyboard::Num5) _grid->setFixedValue(_iSelected, _jSelected, 5);
        if (event->key.code == sf::Keyboard::Num6) _grid->setFixedValue(_iSelected, _jSelected, 6);
        if (event->key.code == sf::Keyboard::Num7) _grid->setFixedValue(_iSelected, _jSelected, 7);
        if (event->key.code == sf::Keyboard::Num8) _grid->setFixedValue(_iSelected, _jSelected, 8);
        if (event->key.code == sf::Keyboard::Num9) _grid->setFixedValue(_iSelected, _jSelected, 9);
        if (event->key.code == sf::Keyboard::Backspace) _grid->setFixedValue(_iSelected, _jSelected, 0);

        if (event->key.code == sf::Keyboard::Left && _jSelected > 0) _jSelected--;
        if (event->key.code == sf::Keyboard::Right && _jSelected < 8) _jSelected++;
        if (event->key.code == sf::Keyboard::Down && _iSelected < 8) _iSelected++;
        if (event->key.code == sf::Keyboard::Up && _iSelected > 0) _iSelected--;
    }
}

void PlayingState::update(State *state)
{
    _grid->update(state);
    _grid->setCellColor(_iSelected, _jSelected, sf::Color(255, 0, 0, 100));
}

void PlayingState::draw(sf::RenderWindow *window)
{
    _grid->draw(window);
}