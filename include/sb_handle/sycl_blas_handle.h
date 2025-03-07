/***************************************************************************
 *
 *  @license
 *  Copyright (C) Codeplay Software Limited
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  For your convenience, a copy of the License has been included in this
 *  repository.
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  SYCL-BLAS: BLAS implementation using SYCL
 *
 *  @filename sycl_blas_handle.h
 *
 **************************************************************************/

#ifndef SYCL_BLAS_HANDLE_H
#define SYCL_BLAS_HANDLE_H
#include "blas_meta.h"
#include "operations/blas1_trees.h"
#include "operations/blas2_trees.h"
#include "operations/blas3_trees.h"
#include "operations/extension/reduction.h"
#include "sycl_blas_helper.h"
namespace blas {

/** SB_Handle.
 * @brief Primary template for the SB_Handle specializations.
 * The SB_Handle represents the object that executes a tree on
 * a specific backend.
 * SB_Handle have state, and they must be instantiated
 * before using them.
 * Only one method is mandatory, the Execute method.
 */
class SB_Handle {
  using queue_t = cl::sycl::queue;

 public:
  using event_t = std::vector<cl::sycl::event>;
  inline SB_Handle(queue_t q)
      : q_(q),
        workGroupSize_(helper::get_work_group_size(q)),
        localMemorySupport_(helper::has_local_memory(q)),
        computeUnits_(helper::get_num_compute_units(q)) {}

  template <typename expression_tree_t>
  event_t execute(expression_tree_t tree);

  template <typename expression_tree_t, typename index_t>
  event_t execute(expression_tree_t tree, index_t localSize);

  template <typename expression_tree_t, typename index_t>
  event_t execute(expression_tree_t tree, index_t localSize,
                  index_t globalSize);
  template <typename expression_tree_t, typename index_t>
  event_t execute(expression_tree_t tree, index_t localSize, index_t globalSize,
                  index_t local_memory_size);

  template <typename operator_t, typename lhs_t, typename rhs_t>
  event_t execute(AssignReduction<operator_t, lhs_t, rhs_t>);

  template <typename operator_t, typename lhs_t, typename rhs_t,
            typename local_memory_t>
  event_t execute(AssignReduction<operator_t, lhs_t, rhs_t> t,
                  local_memory_t scr);

  template <typename input_t, typename output_t, bool DoubleBuffer, bool NbcA,
            bool NbcB, int ClSize, typename tile_type, bool TransA, bool TransB,
            typename element_t, bool is_beta_zero, int GemmMemoryType,
            int GemmAlgorithm, int GemmVectorization, int VectorSize,
            int BatchType, bool UseJointMatrix>
  event_t execute(
      Gemm<input_t, output_t, DoubleBuffer, NbcA, NbcB, ClSize, tile_type,
           TransA, TransB, element_t, is_beta_zero, GemmMemoryType,
           GemmAlgorithm, GemmVectorization, VectorSize, BatchType, UseJointMatrix>
          gemm_tree);

  // Tall and skinny Gemm specialization
  template <typename input_t, typename output_t, bool DoubleBuffer, bool NbcA,
            bool NbcB, int ClSize, typename tile_type, bool TransA, bool TransB,
            typename element_t, bool is_beta_zero, int GemmMemoryType,
            int GemmVectorization, int VectorSize, int BatchType>
  event_t execute(
      Gemm<input_t, output_t, DoubleBuffer, NbcA, NbcB, ClSize, tile_type,
           TransA, TransB, element_t, is_beta_zero, GemmMemoryType,
           static_cast<int>(gemm_algorithm_t::tall_skinny), GemmVectorization,
           VectorSize, BatchType>
          gemm_wrapper);

  // GemmPartial specialization
  template <typename input_t, typename output_t, bool DoubleBuffer, bool NbcA,
            bool NbcB, int ClSize, typename tile_type, bool TransA, bool TransB,
            bool IsFinal, bool IsBetaZero, typename element_t,
            int GemmMemoryType>
  event_t execute(GemmPartial<input_t, output_t, DoubleBuffer, NbcA, NbcB,
                              ClSize, tile_type, TransA, TransB, IsFinal,
                              IsBetaZero, element_t, GemmMemoryType>
                      gemm_partial);

  // Reduction specialization (inner or outer dimension)
  template <typename operator_t, typename params_t, typename input_t,
            typename output_t>
  event_t execute(
      Reduction<operator_t, params_t, input_t, output_t> reduction_wrapper);

  inline bool has_local_memory() const { return localMemorySupport_; }
  inline queue_t get_queue() const { return q_; }

  inline size_t get_work_group_size() const { return workGroupSize_; }

  inline size_t get_num_compute_units() const { return computeUnits_; }

  inline void wait() { q_.wait(); }

  inline void wait(std::vector<cl::sycl::event> evs) {
    cl::sycl::event::wait(evs);
  }

  inline void wait(cl::sycl::event ev) { cl::sycl::event::wait({ev}); }

  /*  @brief waiting for a list of sycl events
 @param first_event  and next_events are instances of sycl::sycl::event
*/
  // this must be in header as the number of event is controlled by user and we
  // dont know howmany permutation can be used by a user
  template <typename first_event_t, typename... next_event_t>
  void inline wait(first_event_t first_event, next_event_t... next_events) {
    cl::sycl::event::wait(concatenate_vectors(first_event, next_events...));
  }

 private:
  queue_t q_;
  const size_t workGroupSize_;
  const bool localMemorySupport_;
  const size_t computeUnits_;
};

}  // namespace blas

#endif  // SYCL_BLAS_HANDLE_H
