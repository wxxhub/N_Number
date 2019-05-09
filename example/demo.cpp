#include <iostream>
#include "n_digital/n_digital.h"

using namespace std;

int main()
{
    int test[3][3] = {
        1, 2, 3,
        4, 5, 0,
        6, 8, 7,
    };

    int goal[3][3] = {
        1, 2, 3,
        6, 4, 5,
        0, 8, 7,
    };

    NDigital *n_digital = new NDigital();
    n_digital->setMap3(test, goal);

    while (n_digital->findNext() != 0)
    {
        n_digital->checkCloseTable();
    }
    n_digital->checkCloseTable();

    return 0;
}