#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <unistd.h>

#include "bomberman/Bomberman.hpp"

int main(int argv, char **argc)
{
    try 
    {
        Bomberman::instance().game_loop();
    }
    catch(Exception &ex)
    {
        std::cout << ex.message() << std::endl;
    }
    return 0;
}