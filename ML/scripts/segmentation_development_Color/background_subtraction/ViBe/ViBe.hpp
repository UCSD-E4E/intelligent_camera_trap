/*
 * ViBe.hpp
 *
 *  Created on: Aug 2, 2013
 *      Author: Gabrielle Trotter & David Muller
 */

#ifndef VIBE_HPP_
#define VIBE_HPP_

#include <vector>
#include <math.h>
#include <utility>
#include <cstdlib>
#include <stdio.h>
#include "Frame.hpp"


#define VIDEO_WIDTH 134
#define VIDEO_HEIGHT 100
#define NUMBER_OF_SAMPLES 20



class ViBe {
public:

	ViBe(int reqMatches, int radius, int subsamplingFactor);
	bool initBg(Frame& frame);
	virtual ~ViBe();
	void updateModel(Frame& frame, Frame& output);

private:
	int nbSamples;
	int reqMatches;
	int radius;
	int subsamplingFactor;
	int width;
	int height;
	unsigned char samples[VIDEO_WIDTH][VIDEO_HEIGHT][NUMBER_OF_SAMPLES];
	unsigned char segmentationMap[VIDEO_WIDTH][VIDEO_HEIGHT];

	float calcDistance(int x, int y, int bg_model_num,Frame& frame);
	void returnNeighbors(int x, int y, std::vector<std::pair<int, int> >& neighbors);


};

#endif /* VIBE_HPP_ */
