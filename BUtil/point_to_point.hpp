#pragma once

#include <BUtil/detail/mpi_types.hpp>

namespace BUtil {

template <typename T>
class PlaceHolderObj;

template <typename T>
class MessageObj {
public:
  PlaceHolderObj<T> operator()(size_t from) {
    from_ = from;
    return PlaceHolderObj<T>(*this);
  }

  MessageObj(T& data, int tag, MPI_Comm comm) : data_(data), tag_(tag),
                                                      comm_(comm) {}

  T& data_;
  size_t to_, from_;
  MPI_Comm comm_;
  int tag_;
};

template <typename T>
class SendoffObj;

template <typename T>
class PlaceHolderObj {
public:
  PlaceHolderObj(MessageObj<T>& obj) : obj_(obj) {}

  SendoffObj<T> operator->() {
    return SendoffObj<T>(obj_);
  }

  MessageObj<T>& obj_;
};

template <typename T>
class SendoffObj {
public:
  SendoffObj(MessageObj<T>& obj) : obj_(obj) {}

  SendoffObj* operator->() {
    return this;
  }

  T to(size_t to) {
    obj_.to_ = to;
    if (BUtil::rank() == obj_.from_) {
      MPI_Send(&obj_.data_, sizeof(T), MPI_CHAR, obj_.to_, obj_.tag_, obj_.comm_);
    } else if (BUtil::rank() == obj_.to_) {
      MPI_Recv(&obj_.data_, sizeof(T), MPI_CHAR, obj_.from_, obj_.tag_, obj_.comm_, MPI_STATUS_IGNORE);
    }
    return obj_.data_;
  }

  MessageObj<T>& obj_;
};


template <typename T>
inline MessageObj<T> message(T& data, int tag = 0,
                             MPI_Comm comm = MPI_COMM_WORLD) {
  static_assert(std::is_trivially_copyable<T>::value, "BUtil::message(): type must be trivially copyable");
  return MessageObj<T>(data, tag, comm);
}

}
