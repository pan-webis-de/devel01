//written by Fabian D.
#pragma once
#include <fstream>
#include "FeatureSet.h"
#include <cstdlib>

class SVMFile
{
public:
	SVMFile(string svmWriteFile);
	~SVMFile();

	void writeFeatureSetToFile(FeatureSet& featureSet);
private:
	fstream outSVMStream;
};

