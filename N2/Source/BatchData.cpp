#include "BatchData.h"

BatchData::BatchData(Mtx44 model) : model(model){}


BatchData::BatchData()
{
}


BatchData::~BatchData()
{
}

void BatchData::setModel(const Mtx44& model)
{
	this->model = model;
}