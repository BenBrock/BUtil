
#include <BUtil/BUtil.hpp>

int main(int argc, char** argv) {
  BUtil::init();
  printf("Hello! I'm rank %lu/%lu\n", BUtil::rank(), BUtil::nprocs());
  BUtil::finalize();
  return 0;
}
