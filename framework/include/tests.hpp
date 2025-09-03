#ifndef _TESTS_HPP_
#define _TESTS_HPP_

#include <iostream>
#include "colors.hpp"

using namespace std;

template <typename T>
void Test(T var1, T var2, size_t line, string testName, int *fail);

int IsFail(int *isFail, string testName);

void SummaryTest();

#endif //_TESTS_HPP_
