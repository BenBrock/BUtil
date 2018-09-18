#pragma once

#include <mpi.h>
#include <BUtil/detail/mpi_types.hpp>

#include <vector>

namespace BUtil {

template <typename T, typename Allocator>
std::vector<T, Allocator> broadcast(const std::vector<T, Allocator>& vals,
                                    size_t root, MPI_Comm comm = MPI_COMM_WORLD) {
  std::vector<T, Allocator> result = vals;
  MPI_Bcast(result.data(), result.size(), get_mpi_type<T>(), root, comm);
  return result;
}

template <typename T, typename Allocator>
std::vector<T, Allocator> allreduce(const std::vector<T, Allocator>& vals,
                                    MPI_Op op, MPI_Comm comm = MPI_COMM_WORLD) {
  std::vector<T, Allocator> result(vals.size());
  MPI_Allreduce(vals.data(), result.data(), vals.size(), get_mpi_type<T>(), op, comm);
  return result;
}

template <typename T, typename Allocator>
std::vector<T, Allocator> reduce(const std::vector<T, Allocator>& vals,
                                 MPI_Op op, size_t root, MPI_Comm comm = MPI_COMM_WORLD) {
  std::vector<T, Allocator> result(vals.size());
  MPI_Reduce(vals.data(), result.data(), vals.size(), get_mpi_type<T>(), op, root, comm);
  return result;
}

}
