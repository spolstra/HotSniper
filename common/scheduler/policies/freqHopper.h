/**
 * Frequency hopper.
 * Used to check the utilisation metric with different frequencies.
 * It will alternate between a low freq and high freq with a fixed interval.
 */

#ifndef __FREQ_HOPPER_H
#define __FREQ_HOPPER_H

#include <vector>
#include "dvfspolicy.h"
#include "simulator.h"

class dvfsFreqHopper : public DVFSPolicy {
public:
    // TODO: check freq and interval types used here
    dvfsFreqHopper(const PerformanceCounters *performanceCounters, int coreRows, int coreColumns, int low_freq, int high_freq, int period);
    virtual std::vector<int> getFrequencies(const std::vector<int> &oldFrequencies, const std::vector<bool> &activeCores);

private:
    const PerformanceCounters *performanceCounters;
    unsigned int coreRows;
    unsigned int coreColumns;
    int low_freq;
    int high_freq;
    int current_freq;
    int period; // in ms
};

#endif // __FREQ_HOPER_H
