#include <iostream>
#include <thread>  
#include <chrono>  

#include "colors.hpp"
#include "ThreadPool.hpp"

using namespace std;
using namespace ilrd_166_7;

/*-----------FORWARD DECLARATON---------------*/
template <typename T>
void Test(T var1, T var2, size_t line, string testName, int *fail);
static int IsFail(int *isFail, string testName);
static void SummaryTest();

int TestRun(const char *testName);
int TestPause(const char *testName);
int TestSetNumOfThreads(const char *testName);

/*-----------CONST GLOBAL VARS---------------*/
const char *NAME = " -- ThreadPool: ";
const char *TEST_FAILED = " test failed ):";
const char *TEST_DONE = " test done (:";
const char SUCCESS = 0;
static int g_num_of_failurs = 0;

const static size_t NUM_OF_CPU = 8;
const static size_t NUM = 3;
mutex g_mutex;
int g_counter = 0;

/*-----------MAIN FUNCTION---------------*/

int main()
{
    size_t num = 0;
    g_num_of_failurs += TestRun("Run");
    for(size_t i = 0; i < 10000000; ++i)
    {
        num += i;
    }

    g_mutex.lock();
    g_counter = 0;
    g_mutex.unlock();
    g_num_of_failurs += TestPause("Pause");
    g_num_of_failurs += TestSetNumOfThreads("Set Num Of Threads");


    SummaryTest();
    return 0;
}

void Producer()
{
    std::lock_guard<mutex> lock(g_mutex);

    for (size_t i = 0; i < NUM; ++i)
    {
        ++g_counter;
    }
}

void Consumer()
{
    std::lock_guard<mutex> lock(g_mutex);

    for (size_t i = 0; i < NUM; ++i)
    {
        --g_counter;
    }
}

void DoStuff()
{
    size_t num = 0;
    for(size_t i = 0; i < 10000000; ++i)
    {
        num += i;
    }
}

 
int TestRun(const char *testName)
{
    int isFail = 0;
    size_t num = 0;
    g_counter = 0;
    ThreadPool tp(NUM_OF_CPU);

    for(size_t i = 0; i < NUM_OF_CPU; ++i)
    {
        tp.AddTask(Producer, ThreadPool::Priority::HIGH);
    }

    for(size_t i = 0; i < NUM_OF_CPU; ++i)
    {
        tp.AddTask(Consumer, ThreadPool::Priority::MEDIUM);
    }
    tp.Run();

    for(size_t i = 0; i < 10000000; ++i)
    {
        num += i;
    }

    g_mutex.lock();
    Test(g_counter, (0), __LINE__, testName, &isFail);
    g_mutex.unlock();

    return IsFail(&isFail, testName);
}

int TestPause(const char *testName)
{
    int isFail = 0;
    size_t num = 0;
    ThreadPool tp(NUM_OF_CPU);
    for(size_t i = 0; i < NUM_OF_CPU; ++i)
    {
        tp.AddTask(Producer, ThreadPool::Priority::HIGH);
    }
    tp.Run();

    for(size_t i = 0; i < 100000000; ++i)
    {
        num += i;
    }

    tp.Pause();

    for(size_t i = 0; i < 100000000; ++i)
    {
        num += i;
    }
    int val = NUM_OF_CPU*NUM;

    g_mutex.lock();
    Test(g_counter, val, __LINE__, testName, &isFail);
    g_mutex.unlock();
    
    for(size_t i = 0; i < NUM_OF_CPU; ++i)
    {
        tp.AddTask(Consumer, ThreadPool::Priority::MEDIUM);
    }
    tp.Run();
    
    for(size_t i = 0; i < 100000; ++i)
    {
        num += i;
    }
    g_mutex.lock();
    Test(g_counter, 0, __LINE__, testName, &isFail);
    g_mutex.unlock();

    return IsFail(&isFail, testName);
}

int TestSetNumOfThreads(const char *testName)
{
    int isFail = 0;
    g_counter = 0;

    ThreadPool tp(NUM_OF_CPU);

    for(size_t i = 0; i < NUM_OF_CPU; ++i)
    {
        tp.AddTask(Producer, ThreadPool::Priority::HIGH);
    }
    tp.Run();

    DoStuff();

    int val = NUM_OF_CPU*NUM;    
    g_mutex.lock();
    Test(g_counter, val, __LINE__, testName, &isFail);
    g_mutex.unlock();

    tp.SetNumOfThreads(NUM_OF_CPU/2);
    for(size_t i = 0; i < NUM_OF_CPU/2; ++i)
    {
        tp.AddTask(Consumer, ThreadPool::Priority::MEDIUM);
    }

    DoStuff();

    val = NUM_OF_CPU*NUM/2;
    g_mutex.lock();
    Test(g_counter, val, __LINE__, testName, &isFail);
    g_mutex.unlock();

    DoStuff();

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

static int IsFail(int *isFail, string testName)
{
    if (!(*isFail))
    {
        cout << GREEN << NAME << testName << TEST_DONE << RESET << endl;
    }

    return *isFail;
}

static void SummaryTest()
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