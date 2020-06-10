#ifndef TESTS_HPP
#define TESTS_HPP

#include <iostream>

class Tests
{
    public:
        bool testAll();
};

struct exceptionData
{
    std::string thrownClass;
    std::string thrownFunction;
    std::string thrownStatement;
};

std::ostream & operator<< (std::ostream &out, const exceptionData &exc);

exceptionData PreperExceptionData(std::string className, std::string functionName, std::string statement);

#endif // TESTS_HPP
