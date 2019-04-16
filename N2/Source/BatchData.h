#ifndef BATCHDATA_H
#define BATCHDATA_H

#include "Mtx44.h"
#include <vector>

class BatchData
{
public:

	BatchData();
	~BatchData();

	std::vector<Mtx44>& getModelMatrices();


private:
	std::vector<Mtx44> modelMatrices;
};

#endif