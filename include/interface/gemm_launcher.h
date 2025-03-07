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
 *  @filename gemm_launcher.h
 *
 **************************************************************************/

#ifndef SYCL_BLAS_BLAS3_GEMM_LAUNCHER_H
#define SYCL_BLAS_BLAS3_GEMM_LAUNCHER_H

#include "operations/blas3_trees.h"
#include "sb_handle/sycl_blas_handle.h"

namespace blas {

/*!
 * @brief Wrapper around Gemm. Creates the views, then makes and launches Gemm
 */
template <int WgSize, bool DoubleBuffer, bool ConflictA, bool ConflictB,
          int ClSize, typename TileT, bool TransA, bool TransB,
          int GemmMemoryType, int GemmAlgorithm, int GemmVectorization,
          bool is_beta_zero, int VectorSize,
          int BatchType = static_cast<int>(gemm_batch_type_t::strided),
          bool UseJointMatrix = false>
struct Gemm_Launcher {
  template <typename sb_handle_t, typename container_0_t,
            typename container_1_t, typename container_2_t, typename element_t,
            typename index_t>
  static typename sb_handle_t::event_t _select_gemm(
      sb_handle_t& sb_handle, index_t _M, index_t _N, index_t _K,
      element_t _alpha, container_0_t a_, index_t _lda, container_1_t b_,
      index_t _ldb, element_t _beta, container_2_t _C, index_t _ldc,
      index_t batch_size);
};

}  // namespace blas

#endif  // SYCL_BLAS_BLAS3_GEMM_LAUNCHER_H
