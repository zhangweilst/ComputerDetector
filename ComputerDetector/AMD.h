#pragma once

#include "CPU.h"

class CAMD : public CCPU
{
public:
	CAMD();
	~CAMD();

// ���ݳ�Ա
protected:

// ������Ա
private:
	void MakeVersionInfo();		// Initialize display Family, Model, Stepping and version string,
												// executed once in the constructor
};
