#ifndef SYCL_UTILS_HPP
#define SYCL_UTILS_HPP

#include <CL/sycl.hpp>
#include <chrono>
#include <tuple>

#include "sycl_blas.h"
#include <common/common_utils.hpp>

// Forward declare methods that we use in `benchmark.cpp`, but define in
// `main.cpp`

namespace blas_benchmark {

// Forward-declaring the function that will create the benchmark
void create_benchmark(Args& args, blas::SB_Handle* sb_handle_ptr, bool* success);

namespace utils {

/**
 * @fn time_event
 * @brief Get the overall run time (start -> end) of a cl::sycl::event enqueued
 * on a queue with profiling.
 */
template <>
inline double time_event<cl::sycl::event>(cl::sycl::event& e) {
  // get start and end times
  cl_ulong start_time = e.template get_profiling_info<
      cl::sycl::info::event_profiling::command_start>();

  cl_ulong end_time = e.template get_profiling_info<
      cl::sycl::info::event_profiling::command_end>();

  // return the delta
  return static_cast<double>(end_time - start_time);
}

}  // namespace utils
}  // namespace blas_benchmark

#endif
