#include "solving-state.hpp"

void SolvingState::init(Grid *grid)
{
    _grid = grid;
}

void SolvingState::addSolution(bitset<9> (&msgf)[9][9])
{
    struct Solution* tmp = new struct Solution;

    for (int i=0; i<9; i++)
    {
        for (int j=0; j<9; j++)
        {
            for (int k=0; k<9; k++)
            {
                if (msgf[i][j][k]) 
                {
                    tmp->values[i][j] = k+1;
                    continue;
                }
            }
        }
    }

    if (_solution != NULL) _solution->next = tmp;
    tmp->prev = _solution;
    _solution = tmp;
}

void SolvingState::cleanSolution()
{
    if (_solution == NULL) return;


    // go to the first solution
    while (_solution->prev != NULL)
    {
        _solution = _solution->prev;
    }

    // free all solutions
    struct Solution* tmp;
    while (_solution->next != NULL)
    {
        tmp = _solution;
        _solution = _solution->next;
        delete tmp;
    }

    delete _solution;
    _solution = NULL;
}

int SolvingState::getNbSolutions()
{
    if (_solution == NULL) return 0;

    while (_solution->prev != NULL)
    {
        _solution = _solution->prev;
    }


    int nbSolutions = 0;
    struct Solution* tmp = _solution;
    while (tmp != NULL)
    {
        nbSolutions++;
        tmp = tmp->next;
    }
    return nbSolutions;
}

void SolvingState::handleInput(sf::Event *event, State *state)
{
    if (event->type == sf::Event::KeyPressed)
    {
        if (event->key.code == sf::Keyboard::Escape) 
        {
            *state = Playing;
            _grid->resetAllValues();
            cleanSolution();
        }

        if (event->key.code == sf::Keyboard::Return)
        {
            _clock.restart();
            bitset<9> msgf[9][9];
            for (int i=0; i<9; i++)
            {
                for (int j=0; j<9; j++)
                {
                    if (_grid->getFixedValue(i,j) > 0)
                    {
                        msgf[i][j].reset();
                        msgf[i][j].set(_grid->getFixedValue(i,j)-1);
                    }
                    else
                    {
                        msgf[i][j].set();
                    }
                }
            }
            solve(msgf);

            cout << "Solving: done!" << endl; 
            cout << getNbSolutions() << " solution(s) found in " << _clock.getElapsedTime().asMilliseconds() << " ms" << std::endl;
        }

        if (event->key.code == sf::Keyboard::Left)
        {
            if ((_solution != NULL) && (_solution->prev != NULL)) _solution = _solution->prev;
        }

        if (event->key.code == sf::Keyboard::Right)
        {
            if ((_solution != NULL) && (_solution->next != NULL)) _solution = _solution->next;
        }
    }
}

void SolvingState::update(State *state)
{
    if (_solution != NULL)
    {
        for (int i=0; i<9; i++)
        {
            for (int j=0; j<9; j++)
            {
                if (_solution->values[i][j] > 0) _grid->setSolvedValue(i,j,_solution->values[i][j]);
            }
        }
    }
    _grid->update(state);
}

void SolvingState::draw(sf::RenderWindow *window)
{
    _grid->draw(window);
}

void SolvingState::solve(bitset<9> (&msgf)[9][9])
{
    int status = iterativeConstraintPropagation(msgf);

    if (status == 2) return; // no solution

    if (status == 1)
    {
        addSolution(msgf);
        return;
    }

    // look for the first ambiguous cell
    int i = 0;
    int j = 0;
    while (msgf[i][j].count() == 1)
    {
        if (j == 8)
        {
            i++;
            j = 0;
        }
        else j++;
    }

    // use copy for backtracking
    bitset<9> copy_msgf[9][9];
    for (int k=0; k<9; k++)
    {
        if (msgf[i][j][k])
        {
            copy(&msgf[0][0], &msgf[0][0] + 81, &copy_msgf[0][0]);
            copy_msgf[i][j].reset();
            copy_msgf[i][j].set(k);
            solve(copy_msgf);
        }
    }
}

int SolvingState::iterativeConstraintPropagation(bitset<9> (&msgf)[9][9])
{
    int status;

    bitset<9> msgbr[9][9];
    bitset<9> msgbc[9][9];
    bitset<9> msgbb[9][9];

    while (true)
    {
        updateBackwardMsg(msgf, msgbr, msgbc, msgbb);
        status = updateForwardMsg(msgf, msgbr, msgbc, msgbb);
        if (status != 0) return status; // stopping criterion = stall (-1), solution (1), or no solution (2)
    }
}

void SolvingState::maxBackwardMsgRow(bitset<9> config, bitset<9> (&msgf)[9][9], int i, int j, int k, int *valid)
{
    if (*valid) return;

    if (config.count() == 9)
    {
        *valid = 1;
        return;
    }

    if (k > 9) return; // detect out of bound

    if (k == j) // detect identical cell
    {
        maxBackwardMsgRow(config, msgf, i, j, k + 1, valid);
        return;
    }
    
    for (int l=0; l<9; l++)
    {
        if ((msgf[i][k][l]) && (~config[l]))
        {
            config.set(l);
            maxBackwardMsgRow(config, msgf, i, j, k + 1, valid);
            config.reset(l);
        }
    }
}


void SolvingState::maxBackwardMsgColumn(bitset<9> config, bitset<9> (&msgf)[9][9], int i, int j, int k, int *valid)
{
    if (*valid) return;

    if (config.count() == 9)
    {
        *valid = 1;
        return;
    }

    if (k > 9) return; // detect out of bound

    if (k == i) // detect same cell
    {
        maxBackwardMsgColumn(config, msgf, i, j, k + 1, valid);
        return;
    }
    
    for (int l=0; l<9; l++)
    {
        if ((msgf[k][j][l]) && (~config[l]))
        {
            config.set(l);
            maxBackwardMsgColumn(config, msgf, i, j, k+ 1, valid);
            config.reset(l);
        }
    }
}

void SolvingState::maxBackwardMsgBlock(bitset<9> config, bitset<9> (&msgf)[9][9], int i, int j, int k, int l, int *valid)
{
    if (*valid) return;

    if (config.count() == 9)
    {
        *valid = 1;
        return;
    }

    if ((k > 9) || (l > 9)) return; // detect out of bound

    if ((k == i) && (l == j)) // detect same cell
    {
        if (l%3 == 2) maxBackwardMsgBlock(config, msgf, i, j, k + 1, l - 2, valid);
        else maxBackwardMsgBlock(config, msgf, i, j, k, l + 1, valid);
        return;
    }
    
    for (int m=0; m<9; m++)
    {
        if ((msgf[k][l][m]) && (~config[m]))
        {
            config.set(m);
            if (l%3 == 2) maxBackwardMsgBlock(config, msgf, i, j, k + 1, l - 2, valid);
            else maxBackwardMsgBlock(config, msgf, i, j, k, l + 1, valid);
            config.reset(m);
        }
    }
}

void SolvingState::updateBackwardMsg(bitset<9> (&msgf)[9][9], bitset<9> (&msgbr)[9][9], bitset<9> (&msgbc)[9][9], bitset<9> (&msgbb)[9][9])
{
    bitset<9> config;
    int valid;

    for (int i=0; i<9; i++)
    {
        for (int j=0; j<9; j++)
        {
            msgbr[i][j].reset();

            for (int k=0; k<9; k++) // go over all possible values of the cell (i,j)
            {
                if (~msgf[i][j][k]) continue; // (k+1) is not a valid value for the cell (i,j)
                config.reset();
                config.set(k);
                valid = 0;
                maxBackwardMsgRow(config, msgf, i, j, 0, &valid);
                if (valid) msgbr[i][j].set(k); // set the backward message for the specific cell (i,j) coming from the row factor                  
            }

            msgbc[i][j].reset();
            for (int k=0; k<9; k++) // go over all possible values of the cell (i,j)
            {
                if (~msgf[i][j][k]) continue; // (k+1) is not a valid value for the cell (i,j)
                config.reset();
                config.set(k);
                valid = 0;
                maxBackwardMsgColumn(config, msgf, i, j, 0, &valid);
                if (valid) msgbc[i][j].set(k); // set the backward message for the specific cell (i,j) coming from the column factor               
            }

            msgbb[i][j].reset();
            for (int k=0; k<9; k++) // go over all possible values of the cell (i,j)
            {
                if (~msgf[i][j][k]) continue; // (k+1) is not a valid value for the cell (i,j)
                config.reset();
                config.set(k);
                valid = 0;
                maxBackwardMsgBlock(config, msgf, i, j, (i/3)*3, (j/3)*3, &valid);
                if (valid) msgbb[i][j].set(k); // set the backward message for the specific cell (i,j) coming from the block factor               
            }
        }
    }
}

int SolvingState::updateForwardMsg(bitset<9> (&msgf)[9][9], bitset<9> (&msgbr)[9][9], bitset<9> (&msgbc)[9][9], bitset<9> (&msgbb)[9][9])
{
    int status = -1;
    int ambiguous = 9 * 9;

    for (int i=0; i<9; i++)
    {
        for (int j=0; j<9; j++)
        {
            for (int k=0; k<9; k++)
            {
                if (~msgf[i][j][k]) continue; // impossible cannot become possible
                if ((~msgbr[i][j][k]) || (~msgbc[i][j][k]) || (~msgbb[i][j][k])) // track changes
                {
                    msgf[i][j].reset(k); 
                    status = 0;
                }
            }
            if (msgf[i][j].count() == 1) ambiguous--;
            if (msgf[i][j].none()) return 2; // no solution
        }
    }
    if (ambiguous == 0) return 1; // solution
    return status;
}