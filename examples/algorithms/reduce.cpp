#include <BUtil/BUtil.hpp>

// Bi-directional exchange allreduce.

template <typename T, typename Fn>
T reduce_recursive(T& value, size_t size, const Fn& fn) {
  if (size == 1) {
    return value;
  }

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

template <typename T, typename Fn>
T reduce(const T& value, const Fn& fn) {
  T reduce_value = value;
  return reduce_recursive(reduce_value, BUtil::nprocs(), fn);
}

int main(int argc, char** argv) {
  BUtil::init();

  size_t val = BUtil::rank();
  size_t reduced_val = reduce(val, std::plus<size_t>{});

  BUtil::print("We reduced the value to rank 0; it is %lu\n", reduced_val);

  BUtil::finalize();
  return 0;
}
