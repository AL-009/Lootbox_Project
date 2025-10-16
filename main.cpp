//main()
// call test1(), test2(), test3() 
#include <iostream>
#include <iomanip>
#include <vector>
#include <array>
#include <algorithm>
#include "randomProject.h"
#include "tests.h"

// Simple ASCII histogram helper
static void printHistogram(const std::array<int, 52>& hist) {
    // Find max count for scaling
    int maxCount = 0;
    for (int k = 1; k <= 51; ++k) {
        maxCount = std::max(maxCount, hist[k]);
    }
    if (maxCount == 0) maxCount = 1;

    std::cout << "\nHistogram: Pulls Until Legendary (1..51)\n";
    std::cout << "(Bars scaled to max bucket)\n";
    for (int k = 1; k <= 51; ++k) {
        double frac = static_cast<double>(hist[k]) / maxCount;
        int barLen = static_cast<int>(std::round(frac * 50)); // up to 50 chars wide
        std::cout << std::setw(2) << k << " | "
                  << std::string(barLen, '#')
                  << " (" << hist[k] << ")\n";
    }
}

int main() {
    // Run simple tests first
    if (!runUnitTests()) {
        std::cerr << "Tests failed. Fix tests before running simulation.\n";
        return 1;
    }

    // --- Simulation parameters ---
    const int NUM_PULLS = 10000;

    LootBoxSimulator sim; // random_device seed by default

    std::vector<int> gaps;            // pulls-until-legendary sequence
    std::array<int, 52> histogram{};  // 1..51 (index 0 unused)

    double effLegendaryRate = sim.simulate(NUM_PULLS, gaps, histogram);

    // --- Reporting ---
    // Effective Legendary probability (should be > 2% due to pity)
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "\n=== Loot Box Simulation ===\n";
    std::cout << "Total pulls: " << NUM_PULLS << "\n";
    std::cout << "Effective Legendary rate: " << (effLegendaryRate * 100.0) << "%\n";

    // Basic stats on gaps
    if (!gaps.empty()) {
        double avg = 0.0;
        int minGap = gaps[0], maxGap = gaps[0];
        long long sum = 0;
        for (int g : gaps) {
            sum += g;
            minGap = std::min(minGap, g);
            maxGap = std::max(maxGap, g);
        }
        avg = static_cast<double>(sum) / gaps.size();

        std::cout << "Number of Legendary events: " << gaps.size() << "\n";
        std::cout << "Average pulls-until-legendary: " << avg << "\n";
        std::cout << "Min gap: " << minGap << ", Max gap: " << maxGap
                  << " (pity caps at 51)\n";
    }

    // ASCII histogram of the geometric-like distribution with pity
    printHistogram(histogram);

    std::cout << "\nNotes:\n"
              << "- Base table: Common=70%, Uncommon=20%, Rare=8%, Legendary=2%.\n"
              << "- Pity: After 50 non-Legendaries in a row, the next (51st) is guaranteed Legendary.\n"
              << "- With no pity, gaps would follow a Geometric(p=0.02). Pity truncates/caps the tail at 51.\n";

    return 0;
}
