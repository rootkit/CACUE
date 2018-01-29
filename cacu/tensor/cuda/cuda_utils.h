/*
 Copyright (c) 2016, David lu
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 * Neither the name of the <organization> nor the
 names of its contributors may be used to endorse or promote products
 derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#ifdef __PARALLELTYPE__
#if __PARALLELTYPE__ == __CUDA__

using namespace std;

namespace cacu {

void cuda_set_device(unsigned int device_id);

void cuda_release();

template<typename DTYPE>
DTYPE* cuda_malloc(size_t length);

template<typename DTYPE>
DTYPE* cuda_malloc_v(size_t length, DTYPE value);

template<typename DTYPE>
void cuda_setvalue(DTYPE *data_, DTYPE value, size_t length);

template<typename DTYPE>
void cuda_copy2dev(DTYPE *d_data_, DTYPE* s_values, size_t length);

template<typename DTYPE>
void cuda_copy2host(DTYPE *d_data_, DTYPE* s_values, size_t length);

template<typename DTYPE>
void cuda_free(DTYPE* data_);

template<typename DTYPE>
void cuda_prsize_t(DTYPE* data_, size_t length);

template<typename DTYPE>
void cuda_refresh(DTYPE *data_, size_t length);

}
#endif
#endif
