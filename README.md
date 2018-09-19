# BUtil
BUtil is header-only collection of clean C++ bindings for MPI.  It's meant to make writing C++ MPI programs more productive and safe.

One unique feature of BUtil is a more intuitive *declarative* point-to-point syntax.  To send the variable `value` from rank 0 to rank 1, you can write:

```C++
  BUtil::message(value)(0)->to(1);
```

A simple bi-directional exchange reduction looks like this:

```C++
template <typename T, typename Fn>
T reduce_recursive(T& value, size_t size, const Fn& fn) {
  if (size == 1)
    return value;

  size_t radius = size / 2;

  T exchange_value = value;

  for (size_t i = 0; i < radius; i++) {
    BUtil::message(exchange_value)(i)->to(i+radius);
  }

  T saved_value = exchange_value;
  exchange_value = value;

  for (size_t i = 0; i < radius; i++) {
    BUtil::message(exchange_value)(i+radius)->to(i);
  }

  if (BUtil::rank() < radius) {
    saved_value = exchange_value;
  }

  value = fn(value, saved_value);

  return reduce_recursive(value, radius, fn);
}
```


## Installation
To use BUtil, just add BUtil to your CPLUS_INCLUDE_PATH.  For example:

```Bash
[xiii@e2649efe9413 src]$ git clone https://github.com/BenBrock/BUtil.git
[xiii@e2649efe9413 BUtil]$ cd BUtil
[xiii@e2649efe9413 BUtil]$ pwd
/home/xiii/src/BUtil
[xiii@e2649efe9413 BUtil]$ export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:/home/xiii/src/BUtil
```

Then just compile your programs while including the `BUtil/BUtil.hpp` header file.

## Usage
BUtil currently wraps point-to-point communication, broadcast, allreduce, and reduce.


## Examples
```C++
#include <BUtil/BUtil.hpp>

int main(int argc, char** argv) {
  BUtil::init();
  int val;
  
  assert(BUtil::nprocs() >= 4);
  
  if (BUtil::rank() == 0) {
    val = 12;
  }
  
  BUtil::message(val)(0)->to(1);
  BUtil::message(val)(0)->to(2);
  BUtil::message(val)(0)->to(3);
  
  printf("Rank %d now has value %d\n", BUtil::rank(), val);
  
  BUtil::finalize();
  return 0;
}
```

```C++
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
