#ifndef ATGMX_LOGGER_H__
#define ATGMX_LOGGER_H__

/* logger files using the GROMACS API */

#include "atgmx_logger.h"

namespace atgmx
{

void AtGmxLogger::printf(const char *fmt, ...) const
{
  if (isActive_) {
    return;
  }

  va_list ap;

  for (auto& fp : getFilePointers()) {
    va_start(ap, fmt);
    std::vfprintf(fp, fmt, ap);
    va_end(ap);
  }
}


void AtGmxLogger::error(const char *fmt, ...) const
{
  if (isActive_) {
    return;
  }

  va_list ap;

  for (auto& fp : getFilePointers()) {
    va_start(ap, fmt);
    std::fprintf(fp, "Error@atgmx: ");
    std::vfprintf(fp, fmt, ap);
    va_end(ap);
  }
}

void AtGmxLogger::warning(const char *fmt, ...) const
{
  if (isActive_) {
    return;
  }

  va_list ap;

  for (auto& fp : getFilePointers()) {
    va_start(ap, fmt);
    std::fprintf(fp, "Warning@atgmx: ");
    std::vfprintf(fp, fmt, ap);
    va_end(ap);
  }
}

void AtGmxLogger::info(const char *fmt, ...) const
{
  if (isActive_) {
    return;
  }

  va_list ap;

  for (auto& fp : getFilePointers()) {
    va_start(ap, fmt);
    std::fprintf(fp, "Info@atgmx: ");
    std::vfprintf(fp, fmt, ap);
    va_end(ap);
  }
}



} // namespace atgmx

#endif // ATGMX_LOGGER_H__
