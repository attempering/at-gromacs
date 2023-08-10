#include "gromacs/utility/exceptions.h"
#include <iostream>
#include <exception>


namespace gmx
{

namespace internal
{

IExceptionInfo::~IExceptionInfo() {}

}

GromacsException::GromacsException(const ExceptionInitializer& details)
{
}

const char* GromacsException::what() const noexcept
{
  return "";
}

void GromacsException::setInfo(const std::type_index& index, internal::ExceptionInfoPointer&& item)
{
}

int InternalError::errorCode() const
{
  return 0;
}

void processExceptionAsFatalError(const std::exception& ex)
{
  std::cerr << ex.what() << '\n';
  throw;
}

}
