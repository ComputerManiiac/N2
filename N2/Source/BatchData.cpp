#include "BatchData.h"



BatchData::BatchData()
{
}


BatchData::~BatchData()
{
}

std::vector<Mtx44>& BatchData::getModelMatrices()
{
	return modelMatrices;
}