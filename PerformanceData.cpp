#include "stdafx.h"
#include "PerformanceData.h"


PerformanceData::PerformanceData(unsigned long memory, unsigned long cpu, unsigned int diskspace) : _memory(memory), _cpu(cpu), _diskspace(diskspace) 
{
}

PerformanceData::~PerformanceData()
{
}

unsigned long PerformanceData::getMemory() const {
	return this->_memory;
}

unsigned long PerformanceData::getCPU() const {
	return this->_cpu;
}

unsigned long PerformanceData::getDiskSpace() const {
	return this->_diskspace;
}
