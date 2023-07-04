/* 
 * Copyright (C) 2010-2023  AT-Engine Developers
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef ATGMX_LOGGER__DEF_H__
#define ATGMX_LOGGER__DEF_H__

#include "../atgmx_engine_context.h"

namespace gmx
{

class AtGmxLogger
{
public:
  AtGmxLogger(const std::string& fnLog,
              bool isActive,
              bool isContinuation) :
    isActive_(isActive),
    fnLog_(fnLog)
  {
    // retrieve the file pointer according to GROMACS's logic
    //std::cout << fnConfig_ << " " << isActive << std::endl;
    if (isActive_) {
      bool appending = isContinuation;
      const char *fileOpeningMode = (appending ? "a" : "w");
      // open with backup functionality
      t_fileio *logFileHandle = gmx_fio_open(fnLog_.c_str(), fileOpeningMode);
      //LogFilePtr logFilePtr = openLogFile(fnLog_.c_str(), appending);
      //t_fileio *logFileHandle = logFilePtr.get();
      fpLog_ = gmx_fio_getfp(logFileHandle);
    }
  }

  ~AtGmxLogger() {
    if (fpLog_ != nullptr) {
      gmx_fio_fclose(fpLog_);
    }
  }

  std::vector<FILE*> getFilePointers() const {
    return std::vector<FILE*>({ stderr, fpLog_ });
  }

  void printf(const char *fmt, ...) const;

  void error(const char *fmt, ...) const;

  void warning(const char *fmt, ...) const;

  void info(const char *fmt, ...) const;

private:
  bool isActive_ = false;
  const std::string& fnLog_;
  FILE* fpLog_ = nullptr;
};

}


#endif
