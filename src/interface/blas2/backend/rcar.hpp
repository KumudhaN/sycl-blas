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
#ifndef SYCL_BLAS_GEMV_RCAR_BACKEND_HPP
#define SYCL_BLAS_GEMV_RCAR_BACKEND_HPP
#include "interface/blas2_interface.h"

namespace blas {
namespace gemv {
namespace backend {
template <transpose_type trn, typename SB_Handle, typename index_t,
          typename element_t, typename container_t0, typename container_t1,
          typename increment_t, typename container_t2>
typename SB_Handle::event_t _gemv(SB_Handle& sb_handle, index_t _M, index_t _N,
                                  element_t _alpha, container_t0 _mA,
                                  index_t _lda, container_t1 _vx,
                                  increment_t _incx, element_t _beta,
                                  container_t2 _vy, increment_t _incy) {
  if (_M >= 512 && trn != transpose_type::Normal) {
    if (_M >= 1024) {
      return blas::internal::_gemv_impl<8, 64, gemv_memory_t::local, trn>(
          sb_handle, _M, _N, _alpha, _mA, _lda, _vx, _incx, _beta, _vy, _incy);
    } else {
      return blas::internal::_gemv_impl<16, 64, gemv_memory_t::local, trn>(
          sb_handle, _M, _N, _alpha, _mA, _lda, _vx, _incx, _beta, _vy, _incy);
    }
  } else {
    return blas::internal::_gemv_impl<32, 64, gemv_memory_t::local, trn>(
        sb_handle, _M, _N, _alpha, _mA, _lda, _vx, _incx, _beta, _vy, _incy);
  }
}
}  // namespace backend
}  // namespace gemv
}  // namespace blas
#endif
