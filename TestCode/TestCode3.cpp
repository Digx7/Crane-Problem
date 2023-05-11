#include<iostream>
#include <cassert>
#include <random>
//#include "cranes_algs.hpp"
#include "cranes_types.hpp"

using namespace std;

int main()
{
    const cranes::step_direction E = cranes::STEP_DIRECTION_EAST,
                               S = cranes::STEP_DIRECTION_SOUTH;
    
    cranes::grid emptyGrid(4,4);
    
    cranes::path filledPath(emptyGrid, {E, E, E});

    cranes::path emptyPath(emptyGrid);

    emptyPath.print();

    //filledPath.print();

    emptyPath = filledPath;

    emptyPath.print();
    
    return 0;
}