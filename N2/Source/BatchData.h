#ifndef BATCHDATA_H
#define BATCHDATA_H

#include "Mtx44.h"

class BatchData
{
public:
	BatchData(Mtx44 model);
	BatchData();
	~BatchData();

	void setModel(const Mtx44& model);

private:
	Mtx44 model;
};

#endif