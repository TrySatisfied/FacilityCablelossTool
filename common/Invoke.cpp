#include "pch.h"
#include "Invoke.h"
#pragma comment(lib, "CommonLib.lib")

Signal* func1(int demod) {
	return CreateSignal(demod);
}