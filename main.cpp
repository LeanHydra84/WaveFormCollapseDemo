#include <iostream>
#include "wfc.hpp"

void add_n_letters(Waves& wv, int n)
{
    for (size_t i = 0; i < n; i++)
    {
        wv.add_droplet('A' + i);
    }
    
}

int main()
{
    srand(time(nullptr));

    Waves waves;
    add_n_letters(waves, 4);

    Board board(20, 20);
    board.collapse(waves);
    
    std::cout << board;


}