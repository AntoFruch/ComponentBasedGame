//
// Created by Antonin Fruchet on 29/05/2026.
//

#ifndef COMPONENT_BASED_ARCH_ILLEGALOPERATIONEXCEPTION_H
#define COMPONENT_BASED_ARCH_ILLEGALOPERATIONEXCEPTION_H
#include <exception>
#include <string>


class IllegalOperationException : public std::runtime_error
{
public:
    explicit IllegalOperationException(const std::string& message);
};


#endif //COMPONENT_BASED_ARCH_ILLEGALOPERATIONEXCEPTION_H
