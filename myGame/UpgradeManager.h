#pragma once
#include "Upgrade.h"
#include <vector>

class UpgradeManager {
public:
    static std::vector<UpgradePtr> getRandomUpgrades(int count);
};