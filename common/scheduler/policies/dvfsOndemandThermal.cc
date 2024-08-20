#include "dvfsOndemandThermal.h"
#include <iomanip>
#include <iostream>

using namespace std;

DVFSOndemandThermal::DVFSOndemandThermal(
                           const PerformanceCounters *performanceCounters,
                           int coreRows, int coreColumns, int minFrequency,
                           int maxFrequency, int frequencyStepSize,
                           float dtmCriticalTemperature,
                           float dtmRecoveredTemperature)
    : performanceCounters(performanceCounters),
      coreRows(coreRows),
      coreColumns(coreColumns),
      minFrequency(minFrequency),
      maxFrequency(maxFrequency),
      currentFrequency(minFrequency),
      frequencyStepSize(frequencyStepSize),
      dtmCriticalTemperature(dtmCriticalTemperature),
      dtmRecoveredTemperature(dtmRecoveredTemperature) {}

vector<int> DVFSOndemandThermal::getFrequencies(
            const vector<int> &oldFrequencies, 
            const vector<bool> &activeCores) {

    if (performanceCounters->getPeakTemperature() > dtmCriticalTemperature) {
        cout << "[Scheduler][ondemand-thermal]: temperature critical -> step down CPU frequency " << endl;
        currentFrequency = step_down(currentFrequency);
    } else if (performanceCounters->getPeakTemperature() < dtmRecoveredTemperature) {
        cout << "[Scheduler][ondemand-thermal]: temperature recovered -> step up CPU frequency " << endl;
        currentFrequency = step_up(currentFrequency);
    } else {
        cout << "[Scheduler][ondemand-thermal]: temperature within accepted range -> CPU frequency unchanged" << endl;
    }

    vector<int> frequencies = calculate_frequencies(activeCores, currentFrequency);
    return frequencies;
}

int DVFSOndemandThermal::step_up(int frequency) const {
    frequency = frequency * 120 / 100;
    frequency = (frequency / frequencyStepSize) *
        frequencyStepSize;  // round
    if (frequency > maxFrequency) {
        frequency = maxFrequency;
    }
    return frequency; 
}

int DVFSOndemandThermal::step_down(int frequency) const {
    frequency = frequency * 80 / 100;
    frequency = (frequency / frequencyStepSize) *
                frequencyStepSize;  // round
    if (frequency < minFrequency) {
        frequency = minFrequency;
    }
    return frequency; 
}

vector<int> DVFSOndemandThermal::calculate_frequencies(
        const vector<bool> &activeCores, int currentFrequency) const {

    // frequency defaults to minFrequency
    vector<int> frequencies(coreRows * coreColumns, minFrequency);

    for (unsigned int coreCounter = 0; coreCounter < coreRows * coreColumns;
            coreCounter++) {
        if (activeCores.at(coreCounter)) {
            float power = performanceCounters->getPowerOfCore(coreCounter);
            float temperature =
                performanceCounters->getTemperatureOfCore(coreCounter);
            cout << "[Scheduler][ondemand_thermal]: Core " << setw(2)
                << coreCounter << ":";
            cout << " P=" << fixed << setprecision(3) << power << " W";
            cout << " f=" << currentFrequency << " MHz";
            cout << " T=" << fixed << setprecision(1) << temperature
                << " C" << endl;
            frequencies.at(coreCounter) = currentFrequency;
        }
    }
    return frequencies;
}
