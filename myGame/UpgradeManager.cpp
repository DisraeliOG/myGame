#include "UpgradeManager.h"
#include <algorithm>
#include <random>

std::vector<UpgradePtr> UpgradeManager::getRandomUpgrades(int count) {
    std::vector<UpgradePtr> allUpgrades = {
            std::make_shared<HealthUpgrade>(),
            std::make_shared<FireRateUpgrade>(),
            std::make_shared<SpeedBoostUpgrade>(),
            std::make_shared<HpRegenUpgrade>(),
            std::make_shared<VampirismUpgrade>(),
            std::make_shared<XPGainUpgrade>()
    };

    std::shuffle(allUpgrades.begin(), allUpgrades.end(), std::default_random_engine(std::random_device{}()));

    if (count > allUpgrades.size()) count = allUpgrades.size();
    return std::vector<UpgradePtr>(allUpgrades.begin(), allUpgrades.begin() + count);
}