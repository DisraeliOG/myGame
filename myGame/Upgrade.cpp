#include "Upgrade.h"
#include "Player.h"

void HealthUpgrade::apply(Player& player) {
    player.increaseMaxHealth(1.05f);
}

void FireRateUpgrade::apply(Player& player) {
    player.decreaseShootDelay(0.95f);  // меньше задержка -> выше скорость
}

void SpeedBoostUpgrade::apply(Player& player) {
    player.increaseSpeed(1.05f);
}

void HpRegenUpgrade::apply(Player& player) {
    player.enableHpRegen(1);
}

void VampirismUpgrade::apply(Player& player) {
    player.enableVampirism(1);
}

void XPGainUpgrade::apply(Player& player) {
    player.increaseXPGainMultiplier(1.25f);
}