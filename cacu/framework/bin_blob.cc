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

#include "bin_blob.h"

namespace cacu {

bin_blob::bin_blob(size_t num, size_t channel, size_t width, size_t height, unsigned int _value,
		phase_type phase) :
blob_base(num, channel, width, height, phase, __bin_blob__) {
	_tdata = new tensor<unsigned int>(_length);
	_s_data = _tdata->pdata();
	if (train == phase) {
		_tdiff = new tensor<float_t>(_length);
		_s_diff = _tdiff->pdata();
	}
}

bin_blob::~bin_blob(){
	delete _tdata;
	if (train == _phase) {
		delete _tdiff;
	}
}

void bin_blob::copy2data(vec_i &data_, size_t i) {
	CHECK_EQ_OP(data_.size(), _cube_length, "blob size must be equal! %d vs %d",
				data_.size(), _cube_length);
	_tdata->copy2data(i*_cube_length, _cube_length, &data_[0]);
}

void bin_blob::copy2data(vec_i &data_) {
	CHECK_EQ_OP(data_.size(), _length, "blob size must be equal! %d vs %d",
				data_.size(), _length);
	_tdata->copy2data(&data_[0]);
}

void bin_blob::copy2diff(vec_t &data_, size_t i) {
	CHECK_EQ_OP(data_.size(), _cube_length, "blob size must be equal! %d vs %d",
				data_.size(), _cube_length);
	_tdiff->copy2data(i*_cube_length, _cube_length, &data_[0]);
}

/*
 * copy data into blob's diff, if blob is established in gpu, io op is needed
 */
void bin_blob::copy2diff(vec_t &data_) {
	CHECK_EQ_OP(data_.size(), _length, "blob size must be equal! %d vs %d",
				data_.size(), _length);
	_tdiff->copy2data(&data_[0]);
}

/*
 * serializa blob_bit data, output data to model file
 * warning: bin_blob just serializa bit wise parameters.
 */
void bin_blob::serializa(std::ostream& os) {
	/*
	unsigned int* s_data_ = (unsigned int*) _s_data;
	bitset<32> _bits;
#if __PARALLELTYPE__ == __CUDA__
	size_t _len = _length / 32;
	if (_length % 32 != 0)
		_len += 1;
	os.write((char*) (&_len), sizeof(_len));
	vec_t _v(_length);
	cuda_copy2host(&_v[0], (float_t*) _s_data, _length);
	size_t _index;

	for (size_t i = 0; i < _v.size(); ++i) {
		_index = i % 32;
		if (_v[i])
			_bits[_index] = 1;
		else
			_bits[_index] = 0;
		if (_index == 31 || i == (_v.size() - 1)) {
			auto w = _bits.to_ulong();
			os.write((char*) (&w), sizeof(w));
		}
	}
	vec_t().swap(_v);
#else
	size_t _len = _length / 32;
	if(_length % 32 != 0)
	_len += 1;
	os.write((char*)(&_len), sizeof(_len));
	size_t _index;
	for(size_t i = 0; i < _length; ++i)
	{
		_index = i % 32;
		if (s_data_[i])
		_bits[_index] = 1;
		else
		_bits[_index] = 0;
		if(_index == 31 || i == (_length - 1)) {
			auto w = _bits.to_ulong();
			os.write((char*)(&w), sizeof(w));
		}
	}
#endif
*/
}

/*
 * load blob_bit data from model file
 */
void bin_blob::load(std::ifstream& is) {
	/*
	unsigned int* s_data_ = (unsigned int*) _s_data;
#if __PARALLELTYPE__ == __CUDA__
	size_t length_;
	is.read(reinterpret_cast<char*>(&length_), sizeof(int));
	size_t _len = _length / 32;
	if (_length % 32 != 0)
		_len += 1;
	CHECK_EQ_OP(length_, _length,
			"parameter length is not equal to local weight: %d vs %d!", length_,
			_len);
	vec_t _v(_length);
	unsigned size_t
	_bit;
	for (size_t i = 0; i < _len; i++) {
		is.read(reinterpret_cast<char*>(&_bit), sizeof(unsigned int));
		bitset<32> bits(_bit);
		for (size_t j = 0; j < 32; ++j) {
			if (bits.test(j) && (i * 32 + j < _length))
				_v[i * 32 + j] = 1;
			else if (!bits.test(j) && (i * 32 + j < _length))
				_v[i * 32 + j] = -1;
		}
	}
	cuda_copy2dev((float_t*) _s_data, &_v[0], length_);
	vec_t().swap(_v);
#else
	size_t length_;
	is.read(reinterpret_cast<char*>(&length_), sizeof(int));
	size_t _len = _length / 32;
	if(_length % 32 != 0)
	_len += 1;
	CHECK_EQ_OP(length_,_length,"parameter length is not equal to local weight: %d vs %d!",length_,_len);
	unsigned size_t _bit;
	for (size_t i = 0; i < _len; i++) {
		is.read(reinterpret_cast<char*>(&_bit), sizeof(unsigned int));
		bitset<32> bits(_bit);
		for(size_t j = 0; j < 32; ++j)
		{
			if(bits.test(j) && (i * 32 + j < _length))
			s_data_[i * 32 + j] = 1;
			else if(!bits.test(j) && (i * 32 + j < _length))
			s_data_[i * 32 + j] = -1;
		}
	}
#endif
*/
}

}