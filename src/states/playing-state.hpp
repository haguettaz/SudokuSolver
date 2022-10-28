#ifndef SUDOKUS_PLAYING_STATE_HPP
#define SUDOKUS_PLAYING_STATE_HPP

#include <SFML/Graphics.hpp>

#include <iostream>

#include "../utils.hpp"
#include "game-state.hpp"

#include "../grid.hpp"

class PlayingState : public GameState
{
public:
    void init(Grid *grid) override;
    void handleInput(sf::Event *event, State *state) override;
    void update(State *state) override;
    void draw(sf::RenderWindow *window) override;

private:
    int _screenWidth, _screenHeight;

    Grid *_grid;

    sf::Font _font;
    sf::Text _cells[9][9];
    sf::RectangleShape _selection;
    int _iSelected = 4; // corresponds to yPos (rows)
    int _jSelected = 4; // corresponds to xPos (columns)
};
#endif // SUDOKUS_PLAYING_STATE_HPP