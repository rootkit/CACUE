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

#ifndef TRAIN_CYCLE_GAN_HPP_
#define TRAIN_CYCLE_GAN_HPP_

#include "../../cacu/cacu.h"

#include "../../cacu/solvers/solvers.h"
//#include "../../cacu/solvers/sgd_solver.h"

#include "../../tools/string_utils.h"
#include "../frcnn/tools_frcnn.h"

#include "../imagenet/data_proc.h"
#include "models.h"

using namespace cacu;

void train_net() {
	int batch_size = 2;

	int max_iter = 1000000;

	int test_iter = 100;
	int train_test_iter = 100;
	int img_size = 256;

	string root_path = "/Users/seallhf/Documents/datasets/cyclegan/datasets/horse2zebra/";

	//set gpu device if training by gpu
#if __USE_DEVICE__ == ON
#if __PARALLELTYPE__ == __CUDA__
	cuda_set_device(0);
#endif
#endif
	//set random seed
	set_rand_seed();

	//log output
	std::ofstream xdlog(root_path + "xdiscriminator.log",
			ios::binary);
	xdlog.precision(std::numeric_limits<cacu::float_t>::digits10);
	//log output
	std::ofstream ydlog(root_path + "ydiscriminator.log",
			ios::binary);
	ydlog.precision(std::numeric_limits<cacu::float_t>::digits10);

	//log output
	std::ofstream glog(root_path + "generator.log",
			ios::binary);
	glog.precision(std::numeric_limits<cacu::float_t>::digits10);




	/*
	 * create generative model
	 */
	blob *xblob_ = cacu_allocator::create_blob(batch_size, 3, img_size, img_size, train);
	blob *x2yblob_ = cacu_allocator::create_blob(batch_size, 3, img_size, img_size, train);
	blob *x2y2xblob_ = cacu_allocator::create_blob(batch_size, 3, img_size, img_size, train);

	blob *yblob_ = cacu_allocator::create_blob(batch_size, 3, img_size, img_size, train);
	blob *y2xblob_ = cacu_allocator::create_blob(batch_size, 3, img_size, img_size, train);
	blob *y2x2yblob_ = cacu_allocator::create_blob(batch_size, 3, img_size, img_size, train);

	network *xgnet = new network();
	network *xdnet = new network();

	network *ygnet = new network();
	network *ydnet = new network();

	layer_block *xgenerator = create_generator_cycleGan(batch_size, 32, train);
	layer_block *ygenerator = create_generator_cycleGan(batch_size, 32, train);

	/*
	 * output of the generator
	 */
	blob *xsuspicious = (blob*)xgenerator->get_oblob();
	blob *ysuspicious = (blob*)ygenerator->get_oblob();

	layer_block *xdiscriminator = create_discriminator_cycleGan(batch_size, 64, train);
	layer_block *ydiscriminator = create_discriminator_cycleGan(batch_size, 64, train);

	/*
	 * output of the discriminator
	 */
	blob *xlogit = (blob*)xdiscriminator->get_oblob();
	blob *ylogit = (blob*)ydiscriminator->get_oblob();

	*xgnet << xgenerator;
	*ygnet << ygenerator;
	*xdnet << xdiscriminator;
	*ydnet << ydiscriminator;

	LOG_DEBUG("Finish creating models!");

//	/*
//	 * create mse_op
//	 */
//	blobs *mse_blobs = new blobs();
//	mse_blobs->push_back(xlogit->copy_create(train,0));
//	mse_blobs->push_back(xlogit->copy_create(train,0));
//
//	mse_loss_op *mse_op = new mse_loss_op(mse_blobs);

	/*
	 * create abse_op
	 */
	blobs *abse_blobs = new blobs();
	abse_blobs->push_back(xsuspicious->copy_create(train,0));
	abse_blobs->push_back(xsuspicious->copy_create(train,0));

	abse_loss_op *abse_op = new abse_loss_op(abse_blobs);
	abse_op->set_loss_weight(10);


	adam_solver *xgsgd = new adam_solver(xgnet);
	xgsgd->set_lr(0.0002);
	xgsgd->set_alpha(0.5);
	adam_solver *ygsgd = new adam_solver(ygnet);
	ygsgd->set_lr(0.0002);
	ygsgd->set_alpha(0.5);
	//gsgd->load_param("/home/haofang/experiment/generative/g_solver.txt");

	adam_solver *xdsgd = new adam_solver(xdnet);
	xdsgd->set_lr(0.0002);
	xdsgd->set_alpha(0.5);
	adam_solver *ydsgd = new adam_solver(ydnet);
	ydsgd->set_lr(0.0002);
	ydsgd->set_alpha(0.5);

	//dsgd->load_param("/home/haofang/experiment/generative/d_solver.txt");

	//string datapath = "/home/haofang/experiment/imagenet/../../data/pascalvoc/VOCdevkit/VOC2012/objects/";
	//string trainlist = "/home/haofang/experiment/imagenet/../../data/pascalvoc/VOCdevkit/VOC2012/object_list.txt";
	string xdatapath = "/Users/seallhf/Documents/datasets/cyclegan/datasets/horse2zebra/trainA/";
	string ydatapath = "/Users/seallhf/Documents/datasets/cyclegan/datasets/horse2zebra/trainB/";
	string xtrainlist = "/Users/seallhf/Documents/datasets/cyclegan/datasets/horse2zebra/trainA.txt";
	string ytrainlist = "/Users/seallhf/Documents/datasets/cyclegan/datasets/horse2zebra/trainB.txt";

	vector<string> xfull_data;
	vector<string> yfull_data;

	vector<string> vec;
	vec_i label(batch_size);

	/**
	 * read train list data into local memory
	 */
	ifstream is = ifstream(xtrainlist);
	is.precision(numeric_limits<float>::digits10);
	if (!is)
		LOG_FATAL("file %s cannot be opened!", xtrainlist.c_str());
	string file_ = "";
	while (getline(is, file_)) {
		xfull_data.push_back(file_);
	}
	is.close();

	random_shuffle(xfull_data.begin(), xfull_data.end());

	/**
	 * read train list data into local memory
	 */
	is = ifstream(ytrainlist);
	is.precision(numeric_limits<float>::digits10);
	if (!is)
		LOG_FATAL("file %s cannot be opened!", ytrainlist.c_str());
	while (getline(is, file_)) {
		yfull_data.push_back(file_);
	}
	is.close();

	random_shuffle(yfull_data.begin(), yfull_data.end());

	int X_ALL_DATA_SIZE = xfull_data.size();
	int Y_ALL_DATA_SIZE = yfull_data.size();

	int step_index_x = 0;
	int step_index_y = 0;
	int step_index_test = 0;
	int step_index_train = 0;
	time_utils *timer = new time_utils();

	unsigned int max_index;
	cacu::float_t count = 0;

	int previous_count=0;
	int allcount = 0;
	cacu::float_t xdlossf = 0,xdlosst = 0,xgloss = 0;
	cacu::float_t ydlossf = 0,ydlosst = 0,ygloss = 0;
	cacu::float_t ccloss;

	for (int i = 1; i <= max_iter; ++i) {

		timer->start();

		/*
		 * read original img data
		 */
		for (int j = 0; j < batch_size; ++j) {
			if (step_index_x == X_ALL_DATA_SIZE) {
				step_index_x = 0;
				random_shuffle(xfull_data.begin(), xfull_data.end());
			}
			if (step_index_y == Y_ALL_DATA_SIZE) {
				step_index_y = 0;
				random_shuffle(xfull_data.begin(), xfull_data.end());
			}
			file_ = xfull_data[step_index_x];
			//load ximage data
			readimg(xblob_->p_data(j),(xdatapath + file_).c_str());

			file_ = yfull_data[step_index_y];
			//load yimage data
			readimg(yblob_->p_data(j),(ydatapath + file_).c_str());

			step_index_x += 1;
			step_index_y += 1;
		}

		/*
		 * adversrial training process
		 */
		xgenerator->layers(0)->get_head_op()->in_data<blob>()->copy_blob(yblob_);
		xgnet->forward_propagate();
		y2xblob_->copy_blob(xsuspicious);

		/*
		 * xdloss_t
		 */
		xdiscriminator->layers(0)->get_head_op()->in_data<blob>()->copy_blob(xblob_);
		xdiscriminator->top_op<sigmoid_with_loss_op>()->in_datas()->astype<bin_blob>(1)->set_data(1);
		xdnet->forward_propagate();
		xdnet->back_propagate();
		xdsgd->updates(i);
		xdlosst = xdiscriminator->top_op<sigmoid_with_loss_op>()->loss();

		/*
		 * xdloss_f
		 */
		xdiscriminator->layers(0)->get_head_op()->in_data<blob>()->copy_blob(y2xblob_);
		xdiscriminator->top_op<sigmoid_with_loss_op>()->in_datas()->astype<bin_blob>(1)->set_data(0);
		xdnet->forward_propagate();
		xdnet->back_propagate();
		xdsgd->updates(i);
		xdlossf = xdiscriminator->top_op<sigmoid_with_loss_op>()->loss();

		xgloss = xdlossf;
		/*
		 * xgloss
		 */
		//copy gradient
		cacu_copy(xdiscriminator->layers(0)->get_head_op()->in_data<blob>()->s_diff(), xgnet->output_blob()->count(), xgnet->output_blob()->s_diff());
		xgnet->back_propagate();
		xgsgd->update_direction(maximize);
		xgsgd->updates(i);

		ygenerator->layers(0)->get_head_op()->in_data<blob>()->copy_blob(xblob_);
		ygnet->forward_propagate();
		x2yblob_->copy_blob(ysuspicious);

		/*
		 * ydloss_t
		 */
		ydiscriminator->layers(0)->get_head_op()->in_data<blob>()->copy_blob(yblob_);
		ydiscriminator->top_op<sigmoid_with_loss_op>()->in_datas()->astype<bin_blob>(1)->set_data(1);
		ydnet->forward_propagate();
		ydnet->back_propagate();
		ydsgd->updates(i);
		ydlosst = ydiscriminator->top_op<sigmoid_with_loss_op>()->loss();

		/*
		 * ydloss_f
		 */
		ydiscriminator->layers(0)->get_head_op()->in_data<blob>()->copy_blob(x2yblob_);
		ydiscriminator->top_op<sigmoid_with_loss_op>()->in_datas()->astype<bin_blob>(1)->set_data(0);
		ydnet->forward_propagate();
		ydnet->back_propagate();
		ydsgd->updates(i);
		ydlossf = ydiscriminator->top_op<sigmoid_with_loss_op>()->loss();

		ygloss = ydlossf;
		/*
		 * ygloss
		 */
		//copy gradient
		cacu_copy(ydiscriminator->layers(0)->get_head_op()->in_data<blob>()->s_diff(), ygnet->output_blob()->count(), ygnet->output_blob()->s_diff());
		ygnet->back_propagate();
		ygsgd->update_direction(maximize);
		ygsgd->updates(i);

		/*
		 * ccloss training process
		 */
		xgsgd->update_direction(minimize);
		ygsgd->update_direction(minimize);

		/*
		 * F(G(y)) -> y
		 */
		xgenerator->layers(0)->get_head_op()->in_data<blob>()->copy_blob(yblob_);
		ygnet->forward_propagate();
		y2xblob_->copy_blob(xsuspicious);

		ygenerator->layers(0)->get_head_op()->in_data<blob>()->copy_blob(y2xblob_);
		ygnet->forward_propagate();
		y2x2yblob_->copy_blob(ysuspicious);

		abse_op->in_datas()->astype<blob>(0)->copy_blob(y2x2yblob_);
		abse_op->in_datas()->astype<blob>(1)->copy_blob(yblob_);

		abse_op->infer();
		ccloss = abse_op->loss();
		abse_op->grad();
		cacu_copy(abse_op->in_datas()->astype<blob>(0)->s_diff(),abse_op->in_datas()->astype<blob>(0)->count(),ysuspicious->s_diff());
		ygnet->back_propagate();
		cacu_copy(ygenerator->layers(0)->get_head_op()->in_data<blob>()->s_diff(),ygenerator->layers(0)->get_head_op()->in_data<blob>()->count(),xsuspicious->s_diff());
		xgnet->back_propagate();

		ygsgd->updates(i);
		xgsgd->updates(i);

		/*
		 * G(F(x)) -> x
		 */
		ygenerator->layers(0)->get_head_op()->in_data<blob>()->copy_blob(xblob_);
		ygnet->forward_propagate();
		x2yblob_->copy_blob(ysuspicious);

		xgenerator->layers(0)->get_head_op()->in_data<blob>()->copy_blob(x2yblob_);
		xgnet->forward_propagate();
		x2y2xblob_->copy_blob(xsuspicious);

		abse_op->in_datas()->astype<blob>(0)->copy_blob(x2y2xblob_);
		abse_op->in_datas()->astype<blob>(1)->copy_blob(xblob_);

		abse_op->infer();
		ccloss += abse_op->loss();
		abse_op->grad();
		cacu_copy(abse_op->in_datas()->astype<blob>(0)->s_diff(),abse_op->in_datas()->astype<blob>(0)->count(),xsuspicious->s_diff());
		xgnet->back_propagate();
		cacu_copy(xgenerator->layers(0)->get_head_op()->in_data<blob>()->s_diff(),xgenerator->layers(0)->get_head_op()->in_data<blob>()->count(),ysuspicious->s_diff());
		ygnet->back_propagate();

		xgsgd->updates(i);
		ygsgd->updates(i);


		xdlog << xdlosst + xdlossf << endl;
		xdlog.flush();

		ydlog << ydlosst + ydlossf << endl;
		ydlog.flush();

		glog << xgloss + ygloss + ccloss << endl;

		timer->end();

		if (i % 1 == 0) {

			LOG_INFO("iter_%d, lr: %f, %ld ms/iter", i, xdsgd->lr(), timer->get_time_span() / 1000);
			LOG_INFO("xdiscriminator loss : (t: %f , f: %f)", xdlosst, xdlossf);
			LOG_INFO("ydiscriminator loss : (t: %f , f: %f)", ydlosst, ydlossf);
			LOG_INFO("generator loss : (x: %f, y: %f, cc: %f)", xgloss, ygloss, ccloss);
		}

		if (i % 500000 == 0){
			xdsgd->set_lr_iter(0.1);
			xgsgd->set_lr_iter(0.1);
			ydsgd->set_lr_iter(0.1);
			ygsgd->set_lr_iter(0.1);
		}

		if (i % 1000 == 0) {
			ostringstream oss;

			oss << root_path << "generative/" << "x_" << i << "_" << img_size << ".jpg";
			imageio_utils::imwrite(((blob*)xgenerator->get_oblob()),oss.str().c_str());

			oss << root_path << "generative/" << "y_" << i << "_" << img_size << ".jpg";
			imageio_utils::imwrite(((blob*)ygenerator->get_oblob()),oss.str().c_str());
		}
		if (i % 10000 == 0) {
			ostringstream oss;

			oss << root_path << "models/" << "xgenerator_" << i << "_" << img_size << ".model";
			xgenerator->save_weights(oss.str());
			oss.str("");

			oss << root_path << "models/" << "ygenerator_" << i << "_" << img_size << ".model";
			ygenerator->save_weights(oss.str());
			oss.str("");

			oss << root_path << "models/" << "xdiscriminator_" << i << "_" << img_size << ".model";
			xdiscriminator->save_weights(oss.str());
			oss.str("");

			oss << root_path << "models/" << "ydiscriminator_" << i << "_" << img_size << ".model";
			ydiscriminator->save_weights(oss.str());

		}
	}

	xdlog.close();
	ydlog.close();
	glog.close();

	LOG_INFO("optimization is done!");

	vector<string>().swap(xfull_data);
	vector<string>().swap(yfull_data);

	delete xdiscriminator,ydiscriminator,xgenerator,ygenerator;
	delete xgnet, ygnet;
	delete xgsgd, ygsgd;
	delete xdnet, ydnet;
	delete xdsgd, ydsgd;
	delete abse_op;
	delete abse_blobs;
	delete xblob_,x2yblob_,x2y2xblob_;
	delete yblob_,y2xblob_,y2x2yblob_;
	delete timer;

#if __USE_DEVICE__ == ON
#if __PARALLELTYPE__ == __CUDA__
	cuda_release();
#endif
#endif
}


#endif
