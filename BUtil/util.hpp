#pragma once

#include <BUtil/BUtil.hpp>

#include <string>

namespace BUtil {

template <typename ...Args>
void print(std::string format, Args... args) {
  fflush(stdout);
  BUtil::barrier();
  if (BUtil::rank() == 0) {
    printf(format.c_str(), args...);
  }
  fflush(stdout);
  BUtil::barrier();
}

}
