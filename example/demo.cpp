#include <iostream>
#include <vector>

#include "n_digital/n_digital.h"

using namespace std;

int main()
{
    vector<vector<int>> origion;
    vector<vector<int>> goal;

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