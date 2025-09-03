#include <iostream>
#include <utility>
#include <unistd.h>
#include <list>

#include "tests.hpp"
#include "colors.hpp"
#include "Framework.hpp"

using namespace std;
using namespace ilrd_166_7;


int TestTemplate(const char *testName);
/*-----------CONST GLOBAL VARS---------------*/
const char *NAME = " -- FrameWork: ";
const char *TEST_FAILED = " test failed ):";
const char *TEST_DONE = " test done (:";
const char SUCCESS = 0;
static int g_num_of_failurs = 0;


const int BUF_SIZ = 100;

enum Key
{
    READ,
    WRITE
};

class ReadArgs : public IKeyArgs
{
public:
    ReadArgs(int fd)
    {
        read(fd, m_buffer, BUF_SIZ);
    }
    int GetKey()
    {
        return READ;
    }
    char *GetBuffer()
    {
        return m_buffer;
    }

private:
    char m_buffer[BUF_SIZ];
};


AsyncInjectionFunction::AsyncInjectionFunction(chrono::milliseconds interval):
                                                m_interval(interval){}

std::chrono::milliseconds AsyncInjectionFunction::GetInterval() const
{
    return m_interval;
}

class CheckRead : public AsyncInjectionFunction
{
    public:
    CheckRead(chrono::milliseconds interval) : AsyncInjectionFunction(interval)
    {
        //test
    }
    bool operator()()
    {
        cout << "Async read check!" << endl;
        return false;
    }
    std::chrono::milliseconds GetInterval() const;
};

class InputProxyTest : public IInputProxy
{
    shared_ptr<IKeyArgs> GetKeyArgs(fdAndMode fdMode)
    {
        return make_shared<ReadArgs>(fdMode.first);
    }
};

class ReadCommand : public ICommand
{
    shared_ptr<AsyncInjectionFunction> Run(shared_ptr<IKeyArgs> args) override
    {
        cout << "Read: " <<static_cast<ReadArgs*>(args.get())->GetBuffer();
        return make_shared<CheckRead>(chrono::milliseconds(1000));
    }
};



shared_ptr<ICommand> CreatorReadCommand()
{   
    return std::make_shared<ReadCommand>();
}


/*-----------MAIN FUNCTION---------------*/
int main()
{
    g_num_of_failurs += TestTemplate("FrameWork");

    SummaryTest();
    return 0;
}

int TestTemplate(const char *testName)
{
    int isFail = -1;
    
    InputProxyTest ipt;
    list<std::pair<fdAndMode, IInputProxy*>> reactorInitList;

    fdAndMode fm{STDIN_FILENO, Mode::READ};
    reactorInitList.push_back(make_pair(fm,&ipt));

    list<std::pair<int, creator_t>> creatorList;
    creatorList.push_back({READ, CreatorReadCommand});

    LinuxListener listener;
    Framework fw(&listener, reactorInitList, creatorList, "stub");
    cout << "Please enter a string to read" << endl;
    fw.Run();

    return 0;

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


