//
//implment 3 testing functions: test1(), test2() and test3()
//make sure each return true/false   
#ifndef TESTS_H
#define TESTS_H

#include <iostream>
#include "randomProject.h"

// Super-lightweight checks (no external framework needed)
#define ASSERT_TRUE(cond, msg) \
    do { if (!(cond)) { std::cerr << "[FAIL] " << msg << "\n"; return false; } } while (0)

inline bool runUnitTests() {
    // 1) Boundary classification tests
    ASSERT_TRUE(LootBoxSimulator::rarityFromRoll(1)   == Rarity::Common,   "1 -> Common");
    ASSERT_TRUE(LootBoxSimulator::rarityFromRoll(70)  == Rarity::Common,   "70 -> Common");
    ASSERT_TRUE(LootBoxSimulator::rarityFromRoll(71)  == Rarity::Uncommon, "71 -> Uncommon");
    ASSERT_TRUE(LootBoxSimulator::rarityFromRoll(90)  == Rarity::Uncommon, "90 -> Uncommon");
    ASSERT_TRUE(LootBoxSimulator::rarityFromRoll(91)  == Rarity::Rare,     "91 -> Rare");
    ASSERT_TRUE(LootBoxSimulator::rarityFromRoll(98)  == Rarity::Rare,     "98 -> Rare");
    ASSERT_TRUE(LootBoxSimulator::rarityFromRoll(99)  == Rarity::Legendary,"99 -> Legendary");
    ASSERT_TRUE(LootBoxSimulator::rarityFromRoll(100) == Rarity::Legendary,"100 -> Legendary");

    // 2) Pity behavior: after 50 non-legendaries, next pull must be Legendary.
    // Use a fixed seed so the first 50 pulls won't randomly include a Legendary.
    LootBoxSimulator sim(12345);
    sim.resetPity();
    // Force 50 non-legendary pulls by overriding randomness:
    // We'll "simulate" this by directly bumping pityCounter via real pulls but
    // restart if we hit Legendary early (unlikely with this seed, but safe).
    int consecNonLegendary = 0;
    while (consecNonLegendary < 50) {
        auto p = sim.pull();
        if (p.rarity == Rarity::Legendary) {
            // start over
            sim.resetPity();
            consecNonLegendary = 0;
        } else {
            ++consecNonLegendary;
        }
    }
    // Now next pull must pity
    auto pityPull = sim.pull();
    ASSERT_TRUE(pityPull.rarity == Rarity::Legendary, "Pity pull should be Legendary");
    ASSERT_TRUE(pityPull.pityApplied == true, "Pity flag should be true");

    std::cerr << "[PASS] All tests passed.\n";
    return true;
}

#endif // TESTS_H
