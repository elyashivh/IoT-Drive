#include <iostream>
#include <dlfcn.h>

#include "Handleton.hpp"
#include "Factory.hpp"


using namespace std;
using namespace ilrd_166_7;


/*-----------MAIN FUNCTION---------------*/
int main()
{
    Factory<int, int, int>* factory1 = Handleton<Factory<int, int, int>>::GetInstance();
    cout << "Address is: " << factory1 << endl;

    void* handle = dlopen("./libhandleton.so", RTLD_LAZY);
    if (!handle)
    {
        cerr << "Error loading library: " << dlerror() << '\n';
        return 0;
    }
    void (*instanceAddr)() = (void (*)()) dlsym (handle, "getInstanceAddr");
    instanceAddr();
    
    dlclose(handle);
    return 0;

}

