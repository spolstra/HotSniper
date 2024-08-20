/**
 * This header implements a ondemand governor that keeps
 * the peak temperature of the cores between dtmRecoveredTemperature and
 * dtmCriticalTemperature by adjusting the frequencies of the 
 * active cores uniformly between minFrequency and maxFrequency.
 */

#ifndef __DVFS_ONDEMAND_THERMAL_H
#define __DVFS_ONDEMAND_THERMAL_H
#include <vector>

#include "dvfspolicy.h"
#include "performance_counters.h"
class DVFSOndemandThermal : public DVFSPolicy {
   public:
    DVFSOndemandThermal(const PerformanceCounters *performanceCounters,
            int coreRows, int coreColumns, int minFrequency, int maxFrequency,
            int frequencyStepSize,
            float dtmCriticalTemperature, float dtmRecoveredTemperature);

    virtual std::vector<int> getFrequencies(
            const std::vector<int> &oldFrequencies, 
            const std::vector<bool> &activeCores);

   private:
    const PerformanceCounters *performanceCounters;
    unsigned int coreRows;
    unsigned int coreColumns;
    int minFrequency;
    int maxFrequency;
    int currentFrequency; // current CPU frequency (active cores only)
    int frequencyStepSize;
    float dtmCriticalTemperature;
    float dtmRecoveredTemperature;

    int step_up(int frequency) const;
    int step_down(int frequency) const;
    std::vector<int> calculate_frequencies(
            const std::vector<bool> &activeCores, int currentFrequency) const;
};
#endif

