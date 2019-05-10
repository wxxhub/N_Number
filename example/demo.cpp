#include <iostream>
#include <vector>

#include "n_digital/n_digital.h"

using namespace std;

int main()
{
    // int origion[3][3] = {
    //     1, 2, 3,
    //     4, 5, 0,
    //     6, 8, 7,
    // };

    // int goal[3][3] = {
    //     1, 2, 3,
    //     6, 4, 5,
    //     0, 8, 7,
    // };

    int origion[3][3];
    int goal[3][3];

    NDigital *n_digital = new NDigital();
    n_digital->getDefault3Map(origion, goal);
    n_digital->setMap3(origion, goal);

    while (n_digital->findNext() != 0)
    {
        n_digital->checkCloseTable();
    }
    n_digital->checkCloseTable();

    return 0;
}