#ifndef SUDOKUS_SPLASHSCREEN_STATE_HPP
#define SUDOKUS_SPLASHSCREEN_STATE_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

#include "game-state.hpp"

#include "../grid.hpp"


class SplashscreenState : public GameState
{
public:
    void init(Grid *grid) override;
    void handleInput(sf::Event *event, State *state) override;
    void update(State *state) override;
    void draw(sf::RenderWindow *window) override;

private:
    sf::Texture _texture;
    sf::Sprite _sprite;
};
#endif // SUDOKUS_SPLASHSCREEN_STATE_HPP