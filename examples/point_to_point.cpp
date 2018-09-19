#include <BUtil/BUtil.hpp>

int main(int argc, char** argv) {
  BUtil::init();
  size_t lel = 0;

  if (BUtil::rank() == 0) {
    lel = 12;
  }

  for (size_t to_rank = 0; to_rank < BUtil::nprocs(); to_rank++) {
    BUtil::message(lel)(0)->to(to_rank);
  }

  BUtil::barrier();

  assert(lel == 12);

  BUtil::print("OK\n");

  BUtil::finalize();
  return 0;
}
