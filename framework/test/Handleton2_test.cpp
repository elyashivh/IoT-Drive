#include <iostream>
#include <dlfcn.h>

#include "handleton.hpp"
#include "factory.hpp"

using namespace std;
using namespace ilrd_166_7;

extern "C" void getInstanceAddr()
{
    Factory<int, int, int>* my_factory = Handleton<Factory<int, int, int>>::GetInstance();
    cout << "Address is: " << my_factory << endl;
}



