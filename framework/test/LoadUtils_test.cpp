
#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <filesystem>
#include "LoadUtils.hpp"

using namespace ilrd_166_7;
using namespace std;

int main()
{
    string path = "/home/elyashiv/Desktop/elyashiv/files";

    DirMonitor monitor(path);
    DllLoader loader;

    Callback<string, DllLoader> cb(&loader, &DllLoader::Load);

    monitor.Register(&cb);
    monitor.Run();

    int x = 0;
    while (x != 1)
    {
        cout << "Press 1 to Exit" << endl;    
        cin >> x;
    }
    
    cout << "Test done" << endl;
    
    return 0;
}
