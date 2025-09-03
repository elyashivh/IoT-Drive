#include <iostream>
#include <memory>
#include <thread>
#include <future>

#include "tests.hpp"
#include "colors.hpp"
#include "singleton.hpp"
#include "factory.hpp"
#include "ThreadPool.hpp"

using namespace std;
using namespace ilrd_166_7;

/*-----------FORWARD DECLARATON---------------*/
int TestTemplate(const char *testName);
int TestMultiThreading(const char *testName);

template <typename T>
void MTTest(T var1, T var2, size_t line, string testName);

/*-----------CONST GLOBAL VARS---------------*/
const char *NAME = " -- Singleton: ";
const char *TEST_FAILED = " test failed ):";
const char *TEST_DONE = " test done (:";
const char SUCCESS = 0;
static int g_num_of_failurs = 0;
static size_t NUM = 0;


Factory<int, int, int>* g_factory = Singleton<Factory<int, int, int>>::GetInstance();
/*-----------MAIN FUNCTION---------------*/
int main()
{
    g_num_of_failurs += TestTemplate("Base");
    g_num_of_failurs += TestMultiThreading("Multi-Threading");

    SummaryTest();
    return 0;
}

int TestTemplate(const char *testName)
{
    int isFail = 0;

    Factory<int, int, int>* factory1 = Singleton<Factory<int, int, int>>::GetInstance();
    Factory<int, int, int>* factory2 = Singleton<Factory<int, int, int>>::GetInstance();

    Test(factory1, factory2, __LINE__, testName, &isFail);

    return IsFail(&isFail, testName);
}

void TaskTest()
{
    Factory<int, int, int>* l_factory = Singleton<Factory<int, int, int>>::GetInstance();

    MTTest(g_factory, l_factory, __LINE__, "Multi-Threading");
}


int TestMultiThreading(const char *testName)
{
    int isFail = 0;
    ThreadPool tp(NUM); 
    
    for (size_t i = 0; i < NUM; ++i)
    {
        tp.AddTask(TaskTest, ThreadPool::Priority::MEDIUM);
    }
    tp.Run();
    

    return IsFail(&isFail, testName);
}





template <typename T>
void Test(T var1, T var2, size_t line, string testName, int *fail)
{
    if (var1 != var2)
    {
        // cout << "var1 " << var1 << endl;
        // cout << "var2 " << var2 << endl;
        cout << RED << line - 2 << NAME << testName << TEST_FAILED << RESET << endl;
        ++(*fail);
    }
}

template <typename T>
void MTTest(T var1, T var2, size_t line, string testName)
{
    if (var1 != var2)
    {
        // cout << "var1 " << var1 << endl;
        // cout << "var2 " << var2 << endl;
        cout << RED << line - 2 << NAME << testName << TEST_FAILED << RESET << endl;
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


