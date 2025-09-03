#include <iostream>
#include <thread>
#include <vector>

#include "WaitableQueue.hpp"
#include "colors.hpp"

using namespace std;
using namespace ilrd_166_7;

/*-----------FORWARD DECLARATON---------------*/
template <typename T>
void Test(T var1, T var2, size_t line, string testName, int *fail);
static int IsFail(int *isFail, string testName);
static void SummaryTest();
int TestIsEmpty(const char *testName);
int TestSingleProducerMultiConsumers(const char *testName);
int TestMultiProducersSingleConsumer(const char *testName);
int TestSingleProducerSingleConsumer(const char *testName);

/*-----------CONST VARS---------------*/
const char *NAME = " -- WaitableQueue: ";
const char *TEST_FAILED = " test failed ):";
const char *TEST_DONE = " test done (:";
const char SUCCESS = 0;
static int g_num_of_failurs = 0;

const static size_t SIZE = 10000;
const static size_t NUM_OF_THREADS = 20;
mutex g_mutex;
size_t g_counter = 0;
/*-----------MAIN FUNCTION---------------*/

int main()
{
    g_num_of_failurs += TestIsEmpty("Is Empty");
    g_num_of_failurs += TestSingleProducerSingleConsumer("Single Producer Single Consumer");
    g_num_of_failurs += TestSingleProducerMultiConsumers("Single Producer Multi Consumers");
    g_num_of_failurs += TestMultiProducersSingleConsumer("Multi Producers Single Consumer");

    SummaryTest();
    return 0;
}

void Producer(WaitableQueue<int> &wq)
{
    g_mutex.lock();
    for (size_t i = 0; i < SIZE; ++i)
    {
        wq.Push(i);
    }
    g_mutex.unlock();
}

void Consumer(WaitableQueue<int> &wq, size_t numOfThreads)
{
    for (size_t i = 0; i < SIZE / numOfThreads; ++i)
    {
        int x = 0;
        wq.Pop(x);
        g_mutex.lock();
        g_counter += x;
        g_mutex.unlock();
    }
}

int TestIsEmpty(const char *testName)
{
    int isFail = 0;
    WaitableQueue<int> wq;

    Test(wq.IsEmpty(), true, __LINE__, testName, &isFail);

    wq.Push(1);
    Test(wq.IsEmpty(), false, __LINE__, testName, &isFail);

    int x = 0;
    wq.Pop(x);
    Test(wq.IsEmpty(), true, __LINE__, testName, &isFail);
    Test(x, 1, __LINE__, testName, &isFail);

    return IsFail(&isFail, testName);
}

int TestSingleProducerSingleConsumer(const char *testName)
{
    int isFail = 0;
    g_counter = 0;
    WaitableQueue<int> wq;
    thread prod(Producer, ref(wq));
    thread cons(Consumer, ref(wq), 1);

    cons.join();
    prod.join();

    size_t totalSum = (SIZE * (SIZE - 1)) / 2;

    Test(totalSum, g_counter, __LINE__, testName, &isFail);

    return IsFail(&isFail, testName);
}

int TestSingleProducerMultiConsumers(const char *testName)
{
    g_counter = 0;
    int isFail = 0;
    size_t numOfThreads = NUM_OF_THREADS;
    WaitableQueue<int> wq;
    thread prod(Producer, ref(wq));
    vector<thread> consVec;

    for (size_t i = 0; i < NUM_OF_THREADS; ++i)
    {
        consVec.emplace_back(Consumer, ref(wq), numOfThreads);
    }

    prod.join();
    for (auto &thread : consVec)
    {
        thread.join();
    }

    size_t totalSum = (SIZE * (SIZE - 1)) / 2;

    Test(totalSum, g_counter, __LINE__, testName, &isFail);

    return IsFail(&isFail, testName);
}

int TestMultiProducersSingleConsumer(const char *testName)
{
    g_counter = 0;
    int isFail = 0;
    
    WaitableQueue<int> wq;
    vector<thread> prodVec;

    for (size_t i = 0; i < NUM_OF_THREADS; ++i)
    {
        prodVec.emplace_back(Producer, ref(wq));
    }
    thread cons(Consumer, ref(wq), 1);

    cons.join();
    for (auto &thread : prodVec)
    {
        thread.join();
    }

    size_t totalSum = (SIZE * (SIZE - 1)) / 2;

    Test(totalSum, g_counter, __LINE__, testName, &isFail);

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
