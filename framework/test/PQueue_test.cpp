#include <iostream>

#include "PQueue.hpp"
#include "colors.hpp"

using namespace std;
using namespace ilrd_166_7;

int main()
{
    PQueue<int> pq;

    for (size_t i = 0; i < 3; ++i)
    {
        pq.push(i);
    }

    if (pq.front() != 1)
    {
        cout << CYAN << "----All tests passed successfully----" << RESET << endl;
    }
    else
    {
        cout << RED << "----Some test failed----" << RESET << endl;
    }

    return 0;
}