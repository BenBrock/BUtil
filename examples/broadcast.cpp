#include <BUtil/BUtil.hpp>

#include <cassert>

int main(int argc, char** argv) {
  BUtil::init();
  constexpr size_t n = 1024;
  std::vector<int> vec(n);

  if (BUtil::rank() == 0) {
    for (size_t i = 0; i < n; i++) {
      vec[i] = i;
    }
  }

  auto new_vec = BUtil::broadcast(vec, 0);

  for (size_t i = 0; i < n; i++) {
    assert(new_vec[i] == i);
  }

  BUtil::barrier();

  BUtil::print("OK\n");

  BUtil::finalize();
  return 0;
}
