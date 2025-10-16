//implement the data types and functions declared in randomProject.h
#include "randomProject.h"
#include <algorithm>

LootBoxSimulator::LootBoxSimulator(unsigned int seed)
    : gen_(seed), dist100_(1, 100), pityCounter_(0), pullsSinceLegendary_(0) {}

void LootBoxSimulator::resetPity() {
    pityCounter_ = 0;
    pullsSinceLegendary_ = 0;
}

Rarity LootBoxSimulator::rarityFromRoll(int roll) {
    // Base table (matches your comments):
    // 1..70   -> Common      (70%)
    // 71..90  -> Uncommon    (20%)
    // 91..98  -> Rare        (8%)
    // 99..100 -> Legendary   (2%)
    if (roll >= 99) return Rarity::Legendary;
    if (roll >= 91) return Rarity::Rare;
    if (roll >= 71) return Rarity::Uncommon;
    return Rarity::Common;
}

const char* LootBoxSimulator::rarityToString(Rarity r) {
    switch (r) {
        case Rarity::Common:    return "Common";
        case Rarity::Uncommon:  return "Uncommon";
        case Rarity::Rare:      return "Rare";
        case Rarity::Legendary: return "Legendary";
    }
    return "Unknown";
}

PullResult LootBoxSimulator::pull() {
    PullResult res{};
    res.roll = dist100_(gen_);
    res.pityApplied = false;

    // If player has failed to get Legendary for 50 consecutive pulls,
    // this (51st) pull is guaranteed Legendary.
    if (pityCounter_ >= 50) {
        res.rarity = Rarity::Legendary;
        res.pityApplied = true;
    } else {
        res.rarity = rarityFromRoll(res.roll);
    }

    // Update pity + gap counter
    ++pullsSinceLegendary_;
    if (res.rarity == Rarity::Legendary) {
        pityCounter_ = 0;
    } else {
        ++pityCounter_;
    }
    return res;
}

double LootBoxSimulator::simulate(
    int numPulls,
    std::vector<int>& gaps,
    std::array<int, 52>& histogram
) {
    std::fill(histogram.begin(), histogram.end(), 0);

    int legendaryCount = 0;
    gaps.clear();
    // Ensure pullsSinceLegendary_ reflects just before first pull
    // (already 0 at construction/reset)
    for (int i = 0; i < numPulls; ++i) {
        auto res = pull();
        if (res.rarity == Rarity::Legendary) {
            ++legendaryCount;
            // Record the geometric-like gap (bounded by pity at 51)
            int gap = std::min(pullsSinceLegendary_, 51);
            if (gap < 1) gap = 1; // safety, shouldn't be needed
            gaps.push_back(gap);
            if (gap >= 1 && gap <= 51) {
                ++histogram[gap];
            }
            pullsSinceLegendary_ = 0; // reset for next streak
        }
    }

    return static_cast<double>(legendaryCount) / static_cast<double>(numPulls);
}
