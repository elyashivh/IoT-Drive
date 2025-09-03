#include <iostream>

#include "tests.hpp"
#include "colors.hpp"
#include "factory.hpp"

using namespace std;
using namespace ilrd_166_7;

/*-----------FORWARD DECLARATON---------------*/
int TestBase(const char *testName);
int TestException(const char *testName);

/*-----------CONST GLOBAL VARS---------------*/
const char *NAME = " -- Factory: ";
const char *TEST_FAILED = " test failed ):";
const char *TEST_DONE = " test done (:";
const char SUCCESS = 0;
static int g_num_of_failurs = 0;


/*-----------MAIN FUNCTION---------------*/
int main()
{
    g_num_of_failurs += TestBase("Base");
    g_num_of_failurs += TestException("Exception");

    SummaryTest();
    return 0;
}

class Base
{
public:
    Base() = default;
    ~Base()= default;
    virtual int  DoSomthing(int x) = 0;
};

class Maor : public Base
{
public:
    Maor(){}
    ~Maor(){}
    virtual int DoSomthing(int x)
    {
        return x * 0;
    }
};

class Yonatan : public Base
{
public:
    Yonatan(){}
    ~Yonatan(){}
    virtual int DoSomthing(int x)
    {
        return x * 1;
    }
};


int TestBase(const char *testName)
{
    int isFail = 0;
    Factory<Base, string> myFactory;

    myFactory.Add("Maor", [](){return make_shared<Maor>();});
    myFactory.Add("Yonatan", [](){return make_shared<Yonatan>();});

    shared_ptr<Base> maorPtr =  myFactory.Create("Maor");
    shared_ptr<Base> yonatnPtr =  myFactory.Create("Yonatan");

    
    int x_maor = maorPtr->DoSomthing(5);
    int x_yoni = yonatnPtr->DoSomthing(5);
    Test(x_maor, 0, __LINE__, testName, &isFail);
    Test(x_yoni, 5, __LINE__, testName, &isFail);

    return IsFail(&isFail, testName);
}

int TestException(const char *testName)
{
    int isFail = 0;
    Factory<Base, string> myFactory;

    myFactory.Add("Maor", [](){return make_shared<Maor>();});
    
    try
    {
        shared_ptr<Base> unknown = myFactory.Create("None");
        ++isFail;
    }
    catch (const std::exception& e)
    {
        cout << GREEN << NAME << testName << " exception caught: " << e.what() << RESET << endl;
    }
    catch (...)
    {
        cout << RED << __LINE__ << NAME << testName << " unknown exception type caught" << RESET << endl;
        ++isFail;
    }

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



