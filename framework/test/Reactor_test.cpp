#include <iostream>
#include <thread>
#include <chrono>
#include <sys/socket.h>
#include <unistd.h>     
#include <cstring>

#include "tests.hpp"
#include "colors.hpp"
#include "Reactor.hpp"


using namespace std;
using namespace ilrd_166_7;

/*-----------FORWARD DECLARATON---------------*/

int TestTemplate(const char *testName);


/*-----------CONST GLOBAL VARS---------------*/
const char *NAME = " -- Reactor";
const char *TEST_FAILED = " test failed ):";
const char *TEST_DONE = " test done (:";
const char SUCCESS = 0;
static int g_num_of_failurs = 0;

static int readTaskCounter = 0;
static int writeTaskCounter = 0;


/*-----------MAIN FUNCTION---------------*/
int main()
{
    g_num_of_failurs += TestTemplate("");

    SummaryTest();
    return 0;
}

int TestTemplate(const char *testName)
{
    int isFail = 0;

    int sv[2]; 
    size_t writeCount = 0;

    socketpair(AF_UNIX, SOCK_STREAM, 0, sv);

    LinuxListener listener;
    Reactor reactor(&listener);

    reactor.Register(sv[1], Mode::READ, [=](int fd,Mode mode) 
    {
        char buffer[6] = {0};
        ssize_t n = read(fd, buffer, 6);
        if (n > 0)
        {
            ++readTaskCounter;
        }
    });


    reactor.Register(sv[0], Mode::WRITE, [&](int fd,Mode mode) 
    {
        const char *msg = "Hello";

        if (writeCount < 5)
        {
            write(fd, msg, strlen(msg));
    
            ++writeTaskCounter;
            ++writeCount;
        }
        else
        {
            reactor.Unregister(fd, Mode::WRITE);
            reactor.Stop();
        }
    });

    reactor.Run();

    Test(readTaskCounter, 5, __LINE__, testName, &isFail);
    Test(writeTaskCounter, 5, __LINE__, testName, &isFail);

    close(sv[0]);
    close(sv[1]);

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


