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

#ifndef OPS_DEFINITION_H_
#define OPS_DEFINITION_H_

namespace cacu {

/**
 * op defination
 * every operation is defined here and regedit in operator_factory.h
 */

enum op_name {
	CACU_INNERPRODUCT,
	CACU_CONVOLUTION,
	CACU_DILATED_CONVOLUTION,
	CACU_BATCH_NORMALIZE,
	CACU_DROPOUT,
	CACU_P_INNERPRODUCT,
	CACU_DECONVOLUTION,
	CACU_LOCAL_CONV,
	CACU_NORM,

	CACU_ROW_MAX_POOLING,
	CACU_MAX_POOLING,
	CACU_AVERAGE_POOLING,
	CACU_ROI_POOLING,

	CACU_RELU,
	CACU_LEAKY_RELU,
	CACU_PRELU,
	CACU_SOFTMAX,
	CACU_TANH,
	CACU_HTANH,
	CACU_SIGMOID,

	CACU_SOFTMAX_LOSS,
	CACU_MSE_LOSS,
	CACU_HINGE_LOSS,
	CACU_MULTI_CE_LOSS,
	CACU_SIGMOID_LOSS,
	CACU_TRIPLET_LOSS,

	CACU_SUM_ELEMWISE,
	CACU_FEATURE_COMBINE,
	CACU_SPLIT,

	CACU_INJECTOR,
	CACU_MATH,

	CACU_PRIMARY_VECTOR,
	CACU_COMBINATION
};


/**
* math op defination
* Introduced by math_op
*/

enum math_op_name {
	ADD,
	SUB,
	MUL,
	DIV,
	ABS,
	MASK_LT,
	MASK_GT
};



}


#endif
