# BUtil
BUtil is a collection of clean C++ bindings for MPI.  It's meant to make writing C++ MPI programs more productive and safe.

To use BUtil, just add BUtil to your CPLUS_INCLUDE_PATH.  For example:

```
[xiii@e2649efe9413 src]$ git clone https://github.com/BenBrock/BUtil.git
[xiii@e2649efe9413 BUtil]$ cd BUtil
[xiii@e2649efe9413 BUtil]$ pwd
/home/xiii/src/BUtil
[xiii@e2649efe9413 BUtil]$ export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:/home/xiii/src/BUtil
```

Then just compile your programs while including the `BUtil/BUtil.hpp` header file.

## Usage
BUtil currently wraps broadcast, allreduce, and reduce for use with C++ STL vectors.  Point-to-point communication is in the works.


## Example
```
#include <BUtil/BUtil.hpp>

int main(int argc, char** argv) {
  BUtil::init();
  printf("Hello! I'm rank %lu/%lu\n", BUtil::rank(), BUtil::nprocs());
  constexpr size_t n = 1024;
  std::vector<int> vec(n, BUtil::rank());
  
  auto reduced_vec = BUtil::allreduce(vec, MPI_SUM);
  
  BUtil::finalize();
}
```
