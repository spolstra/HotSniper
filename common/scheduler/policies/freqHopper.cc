#include "freqHopper.h"
#include <iomanip>
#include <iostream>

using namespace std;

dvfsFreqHopper::dvfsFreqHopper(const PerformanceCounters *performanceCounters, int coreRows, int coreColumns, int low_freq, int high_freq, int period)
	: performanceCounters(performanceCounters), coreRows(coreRows), coreColumns(coreColumns), low_freq(low_freq), high_freq(high_freq), current_freq(low_freq), period(period) {
    cout << "[Scheduler][FreqHopper] low: " << low_freq << endl;
    cout << "[Scheduler][FreqHopper] high: " << high_freq << endl;
    cout << "[Scheduler][FreqHopper] period: " << period << endl;
}

std::vector<int> dvfsFreqHopper::getFrequencies(const std::vector<int> &oldFrequencies, const std::vector<bool> &activeCores) {
	std::vector<int> frequencies(coreRows * coreColumns);

    UInt64 t_ms = Sim()->getClockSkewMinimizationServer()->getGlobalTime().getMS();
    cout << "[Scheduler][FreqHopper]: Time (ms) " << t_ms << endl;
    if (t_ms % period == 0) {
        if (current_freq == low_freq) {
            current_freq = high_freq;
        } else {
            current_freq = low_freq;
        }
    }
    cout << "[Scheduler][FreqHopper] Current freq: " << current_freq << endl;

	for (unsigned int coreCounter = 0; coreCounter < coreRows * coreColumns; coreCounter++) {
		if (activeCores.at(coreCounter)) {
			float power = performanceCounters->getPowerOfCore(coreCounter);
			float temperature = performanceCounters->getTemperatureOfCore(coreCounter);
			int frequency = oldFrequencies.at(coreCounter);
			float utilization = performanceCounters->getUtilizationOfCore(coreCounter);

			cout << "[Scheduler][FreqHopper]: Core " << setw(2) \
                 << coreCounter << ":";
			cout << " P=" << fixed << setprecision(3) << power << " W";
			cout << " f=" << frequency << " MHz";
			cout << " T=" << fixed << setprecision(1) << temperature << " °C";
			cout << " utilization=" << fixed << setprecision(3)
                 << utilization << endl;
		}
		frequencies.at(coreCounter) = current_freq;
	}

	return frequencies;
}
