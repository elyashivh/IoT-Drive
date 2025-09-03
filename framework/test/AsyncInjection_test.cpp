#include <iostream>
#include <functional>
#include <unistd.h>
#include <thread>

#include "tests.hpp"
#include "colors.hpp"
#include "AsyncInjection.hpp"

using namespace std;
using namespace ilrd_166_7;

/*-----------FORWARD DECLARATON---------------*/

int TestAsyncInjection(const char *testName);


/*-----------CONST GLOBAL VARS---------------*/
const char *NAME = " -- AsyncInjection: ";
const char *TEST_FAILED = " test failed ):";
const char *TEST_DONE = " test done (:";
const char SUCCESS = 0;
static int g_num_of_failurs = 0;
static int g_counter = 0;
static int g_num = 10;

/*-----------MAIN FUNCTION---------------*/
int main()
{
    g_num_of_failurs += TestAsyncInjection("Base Case");

    SummaryTest();
    return 0;
}


bool foo()
{
    if (g_num == 0)
    {
        return false;
    }
    else
    {
        ++g_counter;
        --g_num;
        return true;
    }
}


int TestAsyncInjection(const char *testName)
{
    int isFail = 0;
    function<bool()> func = foo;
    AsyncInjection* async = new AsyncInjection(func, chrono::milliseconds(5));
    std::this_thread::sleep_for(std::chrono::milliseconds(110));
    Test(g_counter, 10, __LINE__, testName, &isFail);

    return IsFail(&isFail, testName);
}




template <typename T>
void Test(T var1, T var2, size_t line, string testName, int *fail)
{
    if (var1 != var2)
    {
        cout << "var1 " << var1 << endl;
        cout << "var2 " << var2 << endl;
        cout << RED << line - 2 << NAME << testName << TEST_FAILED << RESET << endl;
        ++(*fail);
    }
}

int IsFail(int *isFail, string testName)
{
    if (!(*isFail))
    {
        cout << GREEN << NAME << testName << TEST_DONE << RESET << endl;
    }

    return *isFail;
}

void SummaryTest()
{
    if (!g_num_of_failurs)
    {
        cout << CYAN << "----All tests passed successfully----" << RESET << endl;
    }
    else
    {
        cout << RED << "----Some test failed----" << RESET << endl;
    }
}


