//
// declare the user-defined datatypes, function prototypes, etc.
#ifndef RANDOMPROJECT_H
#define RANDOMPROJECT_H

#include <random>
#include <vector>
#include <array>
#include <string>

enum class Rarity { Common, Uncommon, Rare, Legendary };

struct PullResult {
    Rarity rarity;
    bool pityApplied;
    int roll; // 1..100, useful for debugging/analysis
};

class LootBoxSimulator {
public:
    // seed is optional: pass a fixed seed in tests for determinism
    explicit LootBoxSimulator(unsigned int seed = std::random_device{}());

    // Perform one pull using current pity state
    PullResult pull();

    // Classify a roll (1..100) into a rarity using the base table
    static Rarity rarityFromRoll(int roll);

    // Human-friendly rarity name
    static const char* rarityToString(Rarity r);

    // Run N pulls. Returns effective Legendary rate in [0,1].
    // - gaps: outputs each "pulls-until-legendary" value (Geometric-like;
    //         the trial that lands Legendary is counted)
    // - histogram: counts indexed by gap size 1..51 (index 0 unused)
    double simulate(
        int numPulls,
        std::vector<int>& gaps,
        std::array<int, 52>& histogram // 1..51 inclusive, pity caps at 51
    );

    // Reset pity state (useful in tests)
    void resetPity();

    // Accessor mainly for tests
    int getPityCounter() const { return pityCounter_; }

private:
    std::mt19937 gen_;
    std::uniform_int_distribution<int> dist100_; // 1..100
    int pityCounter_;                 // consecutive non-Leg pulls (0..50)
    int pullsSinceLegendary_;         // for gap tracking
};

#endif // RANDOMPROJECT_H
