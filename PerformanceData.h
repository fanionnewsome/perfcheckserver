#pragma once
#include <iostream>

class PerformanceData
{
public:
	PerformanceData(unsigned long memory, unsigned long cpu, unsigned int diskspace);
	~PerformanceData();

	unsigned long getMemory() const; 
	unsigned long getCPU() const;
	unsigned long getDiskSpace() const;

private:
	unsigned long _memory;
	unsigned long _cpu;
	unsigned int  _diskspace;
};

