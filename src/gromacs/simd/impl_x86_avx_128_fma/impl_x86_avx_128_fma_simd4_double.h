/*
 * This file is part of the GROMACS molecular simulation package.
 *
 * Copyright (c) 2014,2015, by the GROMACS development team, led by
 * Mark Abraham, David van der Spoel, Berk Hess, and Erik Lindahl,
 * and including many others, as listed in the AUTHORS file in the
 * top-level source directory and at http://www.gromacs.org.
 *
 * GROMACS is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * GROMACS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GROMACS; if not, see
 * http://www.gnu.org/licenses, or write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA.
 *
 * If you want to redistribute modifications to GROMACS, please
 * consider that scientific software is very special. Version
 * control is crucial - bugs must be traceable. We will be happy to
 * consider code for inclusion in the official distribution, but
 * derived work must not be called official GROMACS. Details are found
 * in the README & COPYING files - if they are missing, get the
 * official version at http://www.gromacs.org.
 *
 * To help us fund GROMACS development, we humbly ask that you cite
 * the research papers on the package. Check out http://www.gromacs.org.
 */

#ifndef GMX_SIMD_IMPL_X86_AVX_128_FMA_SIMD4_DOUBLE_H
#define GMX_SIMD_IMPL_X86_AVX_128_FMA_SIMD4_DOUBLE_H

#include "config.h"

#include <immintrin.h>
#include <x86intrin.h>

#include "gromacs/utility/real.h"

#include "impl_x86_avx_128_fma_common.h"
#include "impl_x86_avx_128_fma_simd_double.h"

/* Even if the _main_ SIMD implementation for this architecture file corresponds
 * to 128-bit AVX (since it will be faster), the 256-bit operations will always
 * be available in AVX, so we can use them for double precision SIMD4!
 */
/* SIMD4 Double precision floating point */
#define Simd4Double               __m256d
#define simd4LoadD                 _mm256_load_pd
#define simd4Load1D                _mm256_broadcast_sd
#define simd4Set1D                 _mm256_set1_pd
#define simd4StoreD                _mm256_store_pd
#define simd4LoadUD                _mm256_loadu_pd
#define simd4StoreUD               _mm256_storeu_pd
#define simd4SetZeroD              _mm256_setzero_pd
#define simd4AddD                  _mm256_add_pd
#define simd4SubD                  _mm256_sub_pd
#define simd4MulD                  _mm256_mul_pd
#define simd4FmaddD                _mm256_macc_pd
#define simd4FmsubD                _mm256_msub_pd
#define simd4FnmaddD               _mm256_nmacc_pd
#define simd4FnmsubD               _mm256_nmsub_pd
#define simd4AndD                  _mm256_and_pd
#define simd4AndNotD               _mm256_andnot_pd
#define simd4OrD                   _mm256_or_pd
#define simd4XorD                  _mm256_xor_pd
#define simd4RsqrtD(x)             _mm256_cvtps_pd(_mm_rsqrt_ps(_mm256_cvtpd_ps(x)))
#define simd4AbsD(x)              _mm256_andnot_pd(_mm256_set1_pd(GMX_DOUBLE_NEGZERO), x)
#define simd4NegD(x)              _mm256_xor_pd(x, _mm256_set1_pd(GMX_DOUBLE_NEGZERO))
#define simd4MaxD                  _mm256_max_pd
#define simd4MinD                  _mm256_min_pd
#define simd4RoundD(x)             _mm256_round_pd(x, _MM_FROUND_NINT)
#define simd4TruncD(x)             _mm256_round_pd(x, _MM_FROUND_TRUNC)
#define simd4DotProductD          simd4DotProductD_avx_128_fma
/* SIMD4 booleans corresponding to double */
#define Simd4DBool                __m256d
#define simd4CmpEqD(a, b)           _mm256_cmp_pd(a, b, _CMP_EQ_OQ)
#define simd4CmpLtD(a, b)           _mm256_cmp_pd(a, b, _CMP_LT_OQ)
#define simd4CmpLeD(a, b)           _mm256_cmp_pd(a, b, _CMP_LE_OQ)
#define simd4AndDB                 _mm256_and_pd
#define simd4OrDB                  _mm256_or_pd
#define simd4AnyTrueDB             _mm256_movemask_pd
#define simd4MaskD            _mm256_and_pd
#define simd4MaskNotD(a, sel)  _mm256_andnot_pd(sel, a)
#define simd4BlendD               _mm256_blendv_pd
#define simd4ReduceD               simd4ReduceD_avx_128_fma

static inline double gmx_simdcall
simd4ReduceD_avx_128_fma(__m256d a)
{
    double  f;
    __m128d a0, a1;
    a  = _mm256_hadd_pd(a, a);
    a0 = _mm256_castpd256_pd128(a);
    a1 = _mm256_extractf128_pd(a, 0x1);
    a0 = _mm_add_sd(a0, a1);
    _mm_store_sd(&f, a0);
    return f;
}

static inline double gmx_simdcall
simd4DotProductD_avx_128_fma(__m256d a, __m256d b)
{
    double  d;
    __m128d tmp1, tmp2;
    a    = _mm256_mul_pd(a, b);
    tmp1 = _mm256_castpd256_pd128(a);
    tmp2 = _mm256_extractf128_pd(a, 0x1);

    tmp1 = _mm_add_pd(tmp1, _mm_permute_pd(tmp1, _MM_SHUFFLE2(0, 1)));
    tmp1 = _mm_add_pd(tmp1, tmp2);
    _mm_store_sd(&d, tmp1);
    return d;
}

#endif /* GMX_SIMD_IMPL_X86_AVX_128_FMA_SIMD4_DOUBLE_H */