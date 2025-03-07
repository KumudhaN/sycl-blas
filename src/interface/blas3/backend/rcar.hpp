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
 *  @filename rcar.hpp
 *
 **************************************************************************/
#ifndef SYCL_BLAS_GEMM_RCAR_BACKEND_HPP
#define SYCL_BLAS_GEMM_RCAR_BACKEND_HPP
#include "interface/gemm_launcher.h"

namespace blas {
namespace gemm {
namespace backend {
template <bool _t_a, bool _t_b, bool is_beta_zero, typename SB_Handle,
          typename container_t0, typename container_t1, typename container_t2,
          typename element_t, typename index_t>
typename SB_Handle::event_t _gemm(SB_Handle& sb_handle, index_t _M, index_t _N,
                                  index_t _K, element_t _alpha, container_t0 a_,
                                  index_t _lda, container_t1 b_, index_t _ldb,
                                  element_t _beta, container_t2 _C,
                                  index_t _ldc, index_t batch_size,
                                  gemm_batch_type_t batch_type) {
  if (batch_type == gemm_batch_type_t::interleaved) {
    return blas::Gemm_Launcher<
        64, false, false, false, 64, Tile<4, 4, 4, 4, 1, 1, 1, 1, 4, 4>, _t_a,
        _t_b, static_cast<int>(gemm_memory_t::no_local),
        static_cast<int>(gemm_algorithm_t::standard),
        static_cast<int>(gemm_vectorization_t::full), is_beta_zero, 4,
        static_cast<int>(
            gemm_batch_type_t::interleaved)>::template _select_gemm(sb_handle,
                                                                    _M, _N, _K,
                                                                    _alpha, a_,
                                                                    _lda, b_,
                                                                    _ldb, _beta,
                                                                    _C, _ldc,
                                                                    batch_size);
  }
  if (_M < 512 && _N < 512) {
    return blas::Gemm_Launcher<
        32, false, false, false, 128, Tile<4, 8, 8, 4>, _t_a, _t_b,
        static_cast<int>(gemm_memory_t::local),
        static_cast<int>(gemm_algorithm_t::standard),
        static_cast<int>(gemm_vectorization_t::full), is_beta_zero, 4,
        static_cast<int>(
            gemm_batch_type_t::strided)>::template _select_gemm(sb_handle, _M,
                                                                _N, _K, _alpha,
                                                                a_, _lda, b_,
                                                                _ldb, _beta, _C,
                                                                _ldc,
                                                                batch_size);

  } else {
    return blas::Gemm_Launcher<
        32, false, false, false, 128, Tile<8, 4, 4, 8>, _t_a, _t_b,
        static_cast<int>(gemm_memory_t::local),
        static_cast<int>(gemm_algorithm_t::standard),
        static_cast<int>(gemm_vectorization_t::full), is_beta_zero, 4,
        static_cast<int>(
            gemm_batch_type_t::strided)>::template _select_gemm(sb_handle, _M,
                                                                _N, _K, _alpha,
                                                                a_, _lda, b_,
                                                                _ldb, _beta, _C,
                                                                _ldc,
                                                                batch_size);
  }
}
}  // namespace backend
}  // namespace gemm
}  // namespace blas
#endif
