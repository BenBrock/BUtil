#include <BUtil/BUtil.hpp>

#include <cassert>

size_t correct_sum(size_t n);

int main(int argc, char** argv) {
  BUtil::init();
  constexpr size_t n = 1024;

  std::vector<int> vec(n, BUtil::rank());

  auto new_vec = BUtil::allreduce(vec, MPI_SUM);

  for (size_t i = 0; i < n; i++) {
    assert(new_vec[i] == correct_sum(BUtil::nprocs()));
  }

  BUtil::barrier();

  BUtil::print("OK\n");

  BUtil::finalize();
  return 0;
}

size_t correct_sum(size_t n) {
  size_t sum = 0;
  for (size_t i = 0; i < n; i++) {
    sum += i;
  }
  return sum;
}
