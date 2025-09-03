#include <iostream>

#include "colors.hpp"
// add current hpp

using namespace std;
using namespace ilrd_166_7;

/*-----------FORWARD DECLARATON---------------*/

int TestTemplate(const char *testName);


/*-----------CONST GLOBAL VARS---------------*/
const char *NAME = " -- WaitableQueue: ";
const char *TEST_FAILED = " test failed ):";
const char *TEST_DONE = " test done (:";
const char SUCCESS = 0;
static int g_num_of_failurs = 0;


/*-----------MAIN FUNCTION---------------*/
int main()
{
    g_num_of_failurs += TestTemplate("Is Empty");

    SummaryTest();
    return 0;
}

int TestTemplate(const char *testName)
{
    int isFail = 0;

    //Test(var1, var2, __LINE__, testName, &isFail);

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


