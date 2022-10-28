#include "sudokus.hpp"

Sudokus::Sudokus()
{
    _state = Uninitialized;
    _prevState = Uninitialized;
}

void Sudokus::start()
{
    if (_state != Uninitialized)
        return;

    _window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32), "SUDOKUS");
    _window.setFramerateLimit(10);
    _window.setKeyRepeatEnabled(false);

    _stateInstances[Splashscreen] = new SplashscreenState();
    _stateInstances[Splashscreen]->init(&_grid);
    
    _stateInstances[Playing] = new PlayingState();
    _stateInstances[Playing]->init(&_grid);

    _stateInstances[Solving] = new SolvingState();

    _state = Splashscreen;

    gameLoop();

    auto itr = _stateInstances.begin();
    while (itr != _stateInstances.end())
    {
        delete itr->second;
        itr++;
    }
    _window.close();
}

void Sudokus::gameLoop()
{
    while (_state != Exiting)
    {
        _window.clear(sf::Color(255, 255, 255));

        GameState *state = _stateInstances[_state];

        sf::Event event;
        while (_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                _state = Exiting;

            state->handleInput(&event, &_state);
        }

        if ((_state == Solving) && (_prevState == Playing))
        {
            _stateInstances[Solving]->init(&_grid);
        }

        // Update our entities
        state->update(&_state);

        // Draw our new entities
        state->draw(&_window);

        _window.display();

        _prevState = _state;
    }
}
