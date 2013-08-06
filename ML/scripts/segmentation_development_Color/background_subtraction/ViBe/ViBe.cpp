/*
 * ViBe.cpp
 *
 *  Created on: Aug 2, 2013
 *      Author: Gabrielle Trotter & David Muller
 */

#include "ViBe.hpp"

/**
 * ViBe - Constructor Function
 * @param rMatches minimum number of matches
 * @param rad radius
 * @param subsampFactor amount of random subsampling
 */
ViBe::ViBe(int rMatches, int rad, int subsampFactor) {
	nbSamples =  NUMBER_OF_SAMPLES;
	width = VIDEO_WIDTH;
	height = VIDEO_HEIGHT;
	reqMatches = rMatches;
	radius = rad;
	subsamplingFactor = subsampFactor;

}
/**
 *
 */
ViBe::~ViBe() {
	// TODO Auto-generated destructor stub
}


/***
 * initBG: initialize the background model given a frame from the video camera.
 * @param frame -Frame object from which the model is initialized
 * @return - true on success, false if something went wrong. (what can go wrong?)
 */
bool ViBe::initBg(Frame& frame) {
	int i, j, k, randomIndex, x, y;
	std::vector<std::pair<int, int> > neighbors;

	for (i = 0; i < VIDEO_WIDTH; i++) {
		for (j = 0; j < VIDEO_HEIGHT; j++) {
			ViBe::returnNeighbors(i, j, neighbors);
			for (k = 0; k < NUMBER_OF_SAMPLES; k++) {
				randomIndex = rand() % neighbors.size();
				x = neighbors[randomIndex].first;
				y = neighbors[randomIndex].second;
                //printf("x: %d y: %d",x,y);
				this->samples[i][j][k] = frame.picture[x][y];
			}
		}
	}
	return true;
}



/***
 * calcDistance(int, int, int): Caclulate the grayscale difference between a pixel in the current
 * frame and a pixel in its background model
 * @param x - pixel x coordinate
 * @param y - pixel y coordinate
 * @param bg_model_num - index of background model pixel
 * @param frame - Frame object representing frame from camera
 * @return the difference b/w a pixel in current frame and background model or -1 if the
 * indicies are out of bounds.
 */
float ViBe::calcDistance(int x, int y, int bg_model_num,
		Frame& frame) {
	if (x >= VIDEO_WIDTH || y >= VIDEO_HEIGHT
			|| bg_model_num >= NUMBER_OF_SAMPLES)
		return -1.;
	float dist = float(this->samples[x][y][bg_model_num]) - float(frame.picture[x][y]);
	return dist;
}

/***
 * returnNeighbors(int,int): return the greyscale values
 * of the pixels nieghboring x and y. DON'T TOUCH IT WORKS.
 * @param x - the x coordinate of the pixel you want neighbors of
 * @param y - the y coordinate of the pixel you want neighbors of
 */
void ViBe::returnNeighbors(int x, int y, std::vector<std::pair<int, int> >& neighbors) {
	//typical case (not a corner or edge row/column):
	if (x != 0 && x != VIDEO_WIDTH - 1 && y != 0 && y != VIDEO_HEIGHT - 1) {
		neighbors.push_back(std::make_pair(x - 1, y - 1));
		neighbors.push_back(std::make_pair(x,y - 1));
		neighbors.push_back(std::make_pair(x + 1,y - 1));
		neighbors.push_back(std::make_pair(x - 1, y));
		neighbors.push_back(std::make_pair(x + 1, y));
		neighbors.push_back(std::make_pair(x - 1, y + 1));
		neighbors.push_back(std::make_pair(x, y + 1));
		neighbors.push_back(std::make_pair(x + 1, y + 1));
		return;
	}
	bool flag1 = false;
	bool flag2 = false;

	if (x != 0) {
		neighbors.push_back(std::make_pair(x - 1, y));
		if (y != 0) {
			neighbors.push_back(std::make_pair(x, y - 1));
			neighbors.push_back(std::make_pair(x - 1, y - 1));
			flag1 = true;
		}
		if (y != VIDEO_HEIGHT - 1) {
			neighbors.push_back(std::make_pair(x, y + 1));
			neighbors.push_back(std::make_pair(x - 1, y + 1));
			flag2 = true;
		}
	}
	if (x != VIDEO_WIDTH - 1) {
		neighbors.push_back(std::make_pair(x + 1, y));
		if (y != 0) {
			neighbors.push_back(std::make_pair(x + 1, y - 1));
			if (!flag1) {
				neighbors.push_back(std::make_pair(x,y - 1));
			}
		}
		if (y != VIDEO_HEIGHT - 1) {

			neighbors.push_back(std::make_pair(x + 1, y + 1));
			if (!flag2) {
				neighbors.push_back(std::make_pair(x, y + 1));
			}
		}
	}
	return;
}

void ViBe::updateModel(Frame& frame, Frame& output){
	int i, j, count, index, distance, randomNum, x, y;
	std::vector<std::pair<int, int> > neighbors;
	for (i = 0; i < VIDEO_WIDTH; i++){
		for(j = 0; j < VIDEO_HEIGHT; j++){
			//comparison with the model
			count = 0;
			index = 0;
			distance = 0;
			while ((count < this->reqMatches) && (index < this->nbSamples)){
				distance = ViBe::calcDistance(i, j, index, frame);
				if (distance < this->radius){
					count++;
				}
				index++;
			}
			//pixel classification according to required matches
			if (count >= this->reqMatches){
				//the pixel belongs to the background, store result in segmentation map.
				this->segmentationMap[i][j] = 0;
				output.picture[i][j] = 0;
				randomNum = rand() % this->subsamplingFactor; // how often we replace a pixel in the background model
				if (randomNum == 0){
					randomNum = rand() % this->nbSamples; //randomly choose a background model pixel to replace
					this->samples[i][j][randomNum] = frame.picture[i][j];
				}
				//update neighboring pixel model
				ViBe::returnNeighbors(i, j, neighbors);
				randomNum = rand() % this->subsamplingFactor; //how often we replace a pixel in the background model
				if(randomNum == 0){
					randomNum = rand() % neighbors.size(); //choose a random neighbor
					x = neighbors[randomNum].first;
					y = neighbors[randomNum].second;
					randomNum = rand() % this->nbSamples; //a random pixel from the neighbor's background model
					this->samples[x][y][randomNum] = frame.picture[i][j];
				}
			}
			else{
				//pixel belongs to foreground
				this->segmentationMap[i][j] = 255;
				output.picture[i][j] = 255;
			}

		}
	}
	return;
}
