#include <iostream>
#include <memory>
#include <unistd.h>

#include "tests.hpp"
#include "colors.hpp"
#include "Scheduler.hpp"

using namespace std;
using namespace ilrd_166_7;

/*-----------FORWARD DECLARATON---------------*/

int TestSched(const char *testName);


/*-----------STATIC GLOBAL VARS---------------*/
const char *NAME = " -- Scheduler ";
const char *TEST_FAILED = " test failed ):";
const char *TEST_DONE = " test done (:";
const char SUCCESS = 0;
static int g_num_of_failurs = 0;
static int g_forTest = 0;

struct Inc : public Scheduler::ISchedulerTask
{
    void Execute()
    {
        ++g_forTest;
    }
};

struct TestClass : public Scheduler::ISchedulerTask
{
    TestClass(int var1, int line, string testName):
                    m_var1(var1), m_line(line), m_testName(testName){}
    void Execute()
    {
        if (m_var1 != g_forTest)
        {
            cout << "var1 " << m_var1 << endl;
            cout << "var2 " << g_forTest << endl;
            cout << RED << m_line - 2 << NAME << m_testName << TEST_FAILED << RESET << endl;
        }
    }
private:
    int m_var1;
    int m_line;
    string m_testName;
};



/*-----------MAIN FUNCTION---------------*/
int main()
{
    g_num_of_failurs += TestSched("Base");

    SummaryTest();
    return 0;
}

int TestSched(const char *testName)
{
    static int isFail = 0;
    Scheduler sc;
    
    shared_ptr<Inc> p_inc = std::make_shared<Inc>();
    shared_ptr<TestClass> p_tc = std::make_shared<TestClass>(1, __LINE__, testName);


    Test(0, g_forTest, __LINE__, testName, &isFail);
    sc.AddTask(p_inc, std::chrono::milliseconds(10));
    
    sc.AddTask(p_tc, std::chrono::milliseconds(51));
    sleep(1);
    Test(1, g_forTest, __LINE__, testName, &isFail);

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


