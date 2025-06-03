#include "Upgrade.h"
#include "Player.h"

void HealthUpgrade::apply(Player& player) {
    player.increaseMaxHealth(1.05f);
}

void MegaHealthUpgrade::apply(Player& player) {
    player.increaseMaxHealth(1.20f);
}

void FireRateUpgrade::apply(Player& player) {
    player.decreaseShootDelay(0.95f);  // меньше задержка -> выше скорость
}

void RapidFireUpgrade::apply(Player& player) {
    player.decreaseShootDelay(0.80f);
}

void SpeedBoostUpgrade::apply(Player& player) {
    player.increaseSpeed(1.05f);
}

void SprintBoostUpgrade::apply(Player& player) {
    player.increaseSpeed(1.20f);
}

void HpRegenUpgrade::apply(Player& player) {
    player.enableHpRegen(1);
}

void AdvancedHpRegenUpgrade::apply(Player& player) {
    player.enableHpRegen(3);
}

void VampirismUpgrade::apply(Player& player) {
    player.enableVampirism(1);
}

void GreaterVampirismUpgrade::apply(Player& player) {
    player.enableVampirism(3);
}

void XPGainUpgrade::apply(Player& player) {
    player.increaseXPGainMultiplier(1.25f);
}
void XPSurgeUpgrade::apply(Player& player) {
    player.increaseXPGainMultiplier(2.0f);
}

void ShieldUpgrade::apply(Player& player) {
    player.enableShield();
}

