#include "solving-state.hpp"

void SolvingState::init(Grid *grid)
{
    _grid = grid;

    _stepByStep = true;
    _solved = false;
    _error = false;

    if (!_font.loadFromFile("../assets/fonts/iAWriterDuospace-Regular.otf"))
        std::cout << "Error while loading font" << std::endl;

    for (int i=0; i<9; i++)
    {
        for (int j=0; j<9; j++)
        {
            _cells[i][j].setFont(_font);
            if(_grid->getValue(i, j) == 0)
            {
                _cells[i][j].setCharacterSize(35);
                _cells[i][j].setFillColor(sf::Color::Blue);
                _cells[i][j].setPosition(j*150 + 345, i*150 + 80);
            }
            else
            {
                _cells[i][j].setCharacterSize(150);
                _cells[i][j].setFillColor(sf::Color::Black);
                _cells[i][j].setString(std::to_string(_grid->getValue(i, j)));
                _cells[i][j].setPosition(j*150 + 355, i*150 + 50);
            }
        }
    }

    for (int i=0; i<9; i++)
    {
        for (int j=0; j<9; j++)
        {
            if (_grid->getValue(i,j) > 0) 
            {
                _msgf[i][j].reset();
                _msgf[i][j].set(_grid->getValue(i,j)-1);
                continue;
            }
            _msgf[i][j].set();
        }
    }

    // Init all valid configurations
    std::bitset<9> *ptr = _sets;
    int val;
    std::bitset<9> tmp;

    for (int k=0; k<10; k++)
    {
        val = -1;
        for (int i=0; i<_numSets[k]; i++)
        {
            do
            {
                // val++;
                tmp = std::bitset<9>(val++);
            } while (tmp.count() != k);
            *ptr = tmp;
            ptr++;
            // val++;
        }
    }

    std::cout << "Solver initialisation: done!" << std::endl;
}

int SolvingState::getRightMostSetBit(std::bitset<9> bits)
{
    for (int i=9; i>0; i--)
    {
        if (bits[i-1] == 1)
            return i;
    }
    return 0;
}


std::string SolvingState::getString(int i, int j)
{
    if (_msgf[i][j].count() == 1)
    {
        return std::to_string(getRightMostSetBit(_msgf[i][j]));
    }

    std::string str;
    for (int k=0; k<9; k++)
    {
        if (_msgf[i][j][k]) str += std::to_string(k+1);
        else str += " ";
        if (k == 2 or k == 5) str += "\n";
        else if (k < 8) str += " ";
    }
    return str;
}

void SolvingState::handleInput(sf::Event *event, State *state)
{
    if (event->type == sf::Event::KeyPressed)
    {
        if (event->key.code == sf::Keyboard::Escape) *state = Playing;

        if (event->key.code == sf::Keyboard::Return) _stepByStep = false;

        if (event->key.code == sf::Keyboard::Space && _stepByStep) 
        {
            if (_solved) return;
            updateSolver();
            stopSolver();
        }
;
    }
}

void SolvingState::update(State *state)
{
    for (int i=0; i<9; i++)
    {
        for (int j=0; j<9; j++)
        {
            if (_grid->getValue(i, j) > 0) continue;

            _cells[i][j].setString(getString(i, j));

            if (_msgf[i][j].count() == 1) 
            {
                _cells[i][j].setCharacterSize(150);
                _cells[i][j].setPosition(j*150 + 355, i*150 + 50);
            }
        }
    }

    if (_solved || _error) return;
    if (!_stepByStep)
    {
        _clock.restart();
        while (true)
        {
            if (!updateSolver()) break;
        }
        if (_solved) std::cout << "Solving time: " << _clock.getElapsedTime().asMilliseconds() << "ms" << std::endl;
    }
}

void SolvingState::draw(sf::RenderWindow *window)
{
    _grid->draw(window);
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            window->draw(_cells[i][j]);
        }
    }
}

bool SolvingState::stopSolver()
{
    int maxCount = 0;
    for (int i=0; i<9; i++)
    {
        for (int j=0; j<9; j++)
        {
            if (_msgf[i][j].none()) 
            {
                std::cout << "Error: no solution!" << std::endl;
                return true;
            }
            if (_msgf[i][j].count() > maxCount) maxCount = _msgf[i][j].count();
        }
    }

    if (maxCount == 1) 
    {
        std::cout << "Sudoku solved!" << std::endl;
        _solved = true;
        return true;
    }
    return false;
}

bool SolvingState::updateSolver()
{
    std::bitset<9> msg[9];

    // Rows 
    for (int i=0; i<9; i++)
    {
        for (int j=0; j<9; j++)
        {
            msg[j] = _msgf[i][j];
        }
        computeBackwardMsg(msg);
        for (int j=0; j<9; j++)
        {
            _msgbr[i][j] = msg[j];
        }
    }
    // std::cout << "Row backward messages: done!" << std::endl;

    // Columns
    for (int j=0; j<9; j++)
    {
        for (int i=0; i<9; i++)
        {
            msg[i] = _msgf[i][j];
        }
        computeBackwardMsg(msg);
        for (int i=0; i<9; i++)
        {
            _msgbc[i][j] = msg[i];
        }
    }
    // std::cout << "Columns backward messages: done!" << std::endl;

    // Blocks
    for (int i=0; i<3; i++)
    {
        for (int j=0; j<3; j++)
        {
            for (int bi=0; bi<3; bi++)
            {
                for (int bj=0; bj<3; bj++)
                {
                    msg[bi*3+bj] = _msgf[i*3+bi][j*3+bj];
                }
            }
            computeBackwardMsg(msg);
            for (int bi=0; bi<3; bi++)
            {
                for (int bj=0; bj<3; bj++)
                {
                    _msgbb[i*3+bi][j*3+bj] = msg[bi*3+bj];
                }
            }
        }
    }
    // std::cout << "Block backward messages: done!" << std::endl;

    bool change = false;
    for (int i=0; i<9; i++)
    {
        for (int j=0; j<9; j++)
        {
            if (_msgf[i][j] != (_msgbr[i][j] & _msgbc[i][j] & _msgbb[i][j])) change = true;
            _msgf[i][j] = _msgbr[i][j] & _msgbc[i][j] & _msgbb[i][j];
        }
    }
    std::cout << "Solver iteration: done!" << std::endl;

    if (!change)
    {
        _error = true;
        std::cout << "Sudoku sucks!" << std::endl;
        return false;
    }
    return !stopSolver();
}

void SolvingState::computeBackwardMsg(std::bitset<9> *msg)
{
    // Forward filtering
    bool msgfi[512] = {false}; // 2^9 sets, all zeros at init
    msgfi[0] = true;

    for (int k=1; k < 10; k++) // go over all intern variable levels
    {
        for (int sk=0; sk<_numSets[k]; sk++) // go over all intern variable values
        {
            for (int xk = 0; xk < 9; xk++) // go over all cell values
            {   
                if ((!msg[k-1][xk]) || (!isInSet(xk, _sets[_startSets[k] + sk]))) continue;
                for (int prevsk = 0; prevsk < _numSets[k-1]; prevsk++) // go over all intern variable values
                {
                    if (msgfi[_startSets[k-1] + prevsk] && isValid(xk, _sets[_startSets[k-1] + prevsk], _sets[_startSets[k] + sk]))
                    {
                        msgfi[_startSets[k] + sk] = true;
                        break;
                    }
                }
                if (msgfi[_startSets[k] + sk]) break;
            }
        }
    }

    // Backward filtering
    bool msgbi[512] = {false}; // 2^9 sets, all zeros at init
    msgbi[511] = true;

    for (int k=8; k >= 0; k--) // go over all intern variable levels
    {
        for (int sk=0; sk<_numSets[k]; sk++) // go over all intern variable values
        {
            for (int xk = 0; xk < 9; xk++) // go over all cell values
            {   
                if ((!msg[k][xk]) || isInSet(xk, _sets[_startSets[k] + sk])) continue;
                for (int nextsk = 0; nextsk < _numSets[k+1]; nextsk++) // go over all intern variable values
                {
                    if ((msgbi[_startSets[k+1] + nextsk]) && (isValid(xk, _sets[_startSets[k] + sk], _sets[_startSets[k+1] + nextsk])))
                    {
                        msgbi[_startSets[k] + sk] = true;
                        break;
                    }
                }
                if (msgbi[_startSets[k] + sk]) break;
            }
        }
    }

    // Downward messages
    bool flag;

    for (int k=0; k < 9; k++) // go over all intern variable levels
    {
        for (int xk = 0; xk < 9; xk++) // go over all cell values
        {
            flag = false;
            if (!msg[k][xk]) continue;
            for (int sk=0; sk<_numSets[k]; sk++) // go over all intern variable values
            {   
                if ((!msgfi[_startSets[k] + sk]) || (isInSet(xk, _sets[_startSets[k] + sk]))) continue;
                for (int nextsk = 0; nextsk < _numSets[k+1]; nextsk++) // go over all intern variable values
                {
                    if ((msgbi[_startSets[k+1] + nextsk]) && (isValid(xk, _sets[_startSets[k] + sk], _sets[_startSets[k+1] + nextsk]))) 
                    {
                        flag=true;
                        break;
                    }
                }
                if (flag) break;
            }
            if (flag) continue;
            msg[k].reset(xk);
        }
    }
}

bool SolvingState::isInSet(int val, std::bitset<9> set)
{
    return (set[val] == 1);
}

bool SolvingState::isValid(int val, std::bitset<9> prevSet, std::bitset<9> set)
{
    if (set[val] == 0) return false;
    if (prevSet[val] == 1) return false;

    std::bitset<9> mask = 0;
    mask[val] = 1;
    return (prevSet^mask) == set;
}