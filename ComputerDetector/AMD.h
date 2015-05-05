#pragma once

#include "CPU.h"

class CAMD : public CCPU
{
public:
	CAMD();
	~CAMD();

// 数据成员
protected:

// 函数成员
private:
	void MakeVersionInfo();		// Initialize display Family, Model, Stepping and version string,
												// executed once in the constructor
};
