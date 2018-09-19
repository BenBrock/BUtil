#pragma once

#include <mpi.h>

namespace BUtil {

inline size_t rank(MPI_Comm comm = MPI_COMM_WORLD) {
  int rank;
  MPI_Comm_rank(comm, &rank);
  return rank;
}

inline size_t nprocs(MPI_Comm comm = MPI_COMM_WORLD) {
  int nprocs;
  MPI_Comm_size(comm, &nprocs);
  return nprocs;
}

inline void barrier(MPI_Comm comm = MPI_COMM_WORLD) {
  MPI_Barrier(comm);
}

inline bool mpi_initialized() {
  int flag;
  MPI_Initialized(&flag);
  return flag;
}

inline bool mpi_finalized() {
  int flag;
  MPI_Finalized(&flag);
  return flag;
}

inline void init() {
  if (!mpi_initialized()) {
    MPI_Init(NULL, NULL);
  }
}

inline void finalize() {
  if (!mpi_finalized()) {
    BUtil::barrier();
    MPI_Finalize();
  }
}

}

#include <BUtil/util.hpp>
#include <BUtil/point_to_point.hpp>
#include <BUtil/collectives.hpp>
