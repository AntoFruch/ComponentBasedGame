//
// Created by Antonin Fruchet on 29/05/2026.
//

#include "IllegalOperationException.h"

IllegalOperationException::IllegalOperationException(const std::string& message) : std::runtime_error(message)
{
}