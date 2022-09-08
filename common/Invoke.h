#pragma once
#include "signal.h"
class Invoke
{
public:
	Invoke() {};
	~Invoke() {};
};

extern "C" __declspec(dllexport)Signal * func1(int);