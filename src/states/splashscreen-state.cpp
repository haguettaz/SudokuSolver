#include "splashscreen-state.hpp"

void SplashscreenState::init(Grid *grid)
{
    // Declare and load a texture
    if (!_texture.loadFromFile("../assets/images/splashscreen.png"))
        std::cout << "Error while loading texture" << std::endl;

    // Create a sprite
    _sprite.setTexture(_texture);
}

void SplashscreenState::handleInput(sf::Event *event, State *state)
{
    if (event->type == sf::Event::KeyPressed or event->type == sf::Event::MouseButtonPressed)
        *state = Playing;
}

void SplashscreenState::update(State *state) {}
void SplashscreenState::draw(sf::RenderWindow *window)
{
    window->draw(_sprite);
}
