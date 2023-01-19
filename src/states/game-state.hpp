#ifndef SUDOKUS_GAME_STATE_HPP
#define SUDOKUS_GAME_STATE_HPP

#include <SFML/Graphics.hpp>

#include "../grid.hpp"
#include "../utils.hpp"

class GameState
{
public:
    virtual ~GameState();
    virtual void init(Grid *grid) = 0;
    virtual void handleInput(sf::Event *event, State *state) = 0;
    virtual void update(State *state) = 0;
    virtual void draw(sf::RenderWindow *window) = 0;
};


#endif // SUDOKUS_GAME_STATE_HPP