#include <iostream>

#include "tests.hpp"
#include "colors.hpp"
#include "Dispatcher.hpp"


using namespace std;
using namespace ilrd_166_7;

/*-----------FORWARD DECLARATON---------------*/
int TestNotify(const char *testName);
int TestNotifyDeath(const char *testName);
int TestUnRegister(const char *testName);
int TestMultipleNotify(const char *testName);

/*-----------CONST GLOBAL VARS---------------*/
const char *NAME = " -- Dispatcher: ";
const char *TEST_FAILED = " test failed ):";
const char *TEST_DONE = " test done (:";
const char SUCCESS = 0;
static int g_num_of_failurs = 0;


class Observer
{
public:
    Observer(size_t x = 10): m_x(x){}
    void SetX(size_t x){m_x = x;}
    void AddY(size_t y){m_x += y;}
    int GetX(){return m_x;}
    void IsDead(){m_x = 0;}

private:
    int m_x;
};

/*-----------MAIN FUNCTION---------------*/
int main()
{
    g_num_of_failurs += TestNotify("Notify");
    g_num_of_failurs += TestNotifyDeath("Notify-Death");
    g_num_of_failurs += TestUnRegister("UnRegister");
    g_num_of_failurs += TestMultipleNotify("Multiple-Notify");
    
    SummaryTest();
    return 0;
}

int TestNotify(const char *testName)
{
    int isFail = 0;
    Observer ob(10);

    Dispatcher<int> disp;
    Callback<int, Observer> cb(&ob, &Observer::AddY, &Observer::IsDead);
    cb.SetDispatcher(&disp);
    disp.Register(&cb);

    disp.Notify(5);
    Test(ob.GetX(), 15, __LINE__, testName, &isFail);

    disp.Notify(5);
    Test(ob.GetX(), 20, __LINE__, testName, &isFail);

    return IsFail(&isFail, testName);
}

int TestNotifyDeath(const char *testName)
{
    int isFail = 0;
    Observer ob(10);
    Callback<int, Observer> cb(&ob, &Observer::AddY, &Observer::IsDead);

    {
        Dispatcher<int> disp;
        cb.SetDispatcher(&disp);
        disp.Register(&cb);
    
        disp.Notify(5);
        Test(ob.GetX(), 15, __LINE__, testName, &isFail);
    }
    Test(ob.GetX(), 0, __LINE__, testName, &isFail);
    return IsFail(&isFail, testName);
}

int TestUnRegister(const char *testName)
{
    int isFail = 0;
    Observer ob(10);

    Dispatcher<int> disp;
    Callback<int, Observer> cb(&ob, &Observer::AddY, &Observer::IsDead);
    cb.SetDispatcher(&disp);
    disp.Register(&cb);

    disp.Notify(5);
    Test(ob.GetX(), 15, __LINE__, testName, &isFail);

    disp.Unregister(&cb);
    disp.Notify(5);
    Test(ob.GetX(), 15, __LINE__, testName, &isFail);

    return IsFail(&isFail, testName);
}

int TestMultipleNotify(const char *testName)
{
    int isFail = 0;
    Observer ob1(10);
    Observer ob2(5);
    Dispatcher<int> disp;
    Callback<int, Observer> cb1(&ob1, &Observer::AddY, &Observer::IsDead);
    Callback<int, Observer> cb2(&ob2, &Observer::AddY, &Observer::IsDead);
    
    cb1.SetDispatcher(&disp);
    cb2.SetDispatcher(&disp);

    disp.Register(&cb1);
    disp.Register(&cb2);

    disp.Notify(5);
    Test(ob1.GetX(), 15, __LINE__, testName, &isFail);
    Test(ob2.GetX(), 10, __LINE__, testName, &isFail);

    disp.Notify(100);

    Test(ob1.GetX(), 115, __LINE__, testName, &isFail);
    Test(ob2.GetX(), 110, __LINE__, testName, &isFail);

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

