#pragma once
#include <string>
#include <memory>

class Player;

class Upgrade {
public:
    virtual ~Upgrade() = default;
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual void apply(Player& player) = 0;
};

using UpgradePtr = std::shared_ptr<Upgrade>;

class HealthUpgrade : public Upgrade {
public:
    std::string getName() const override { return "Max Health +5%"; }
    std::string getDescription() const override { return "Increases max health by 5%."; }
    void apply(Player& player) override;
};

class FireRateUpgrade : public Upgrade {
public:
    std::string getName() const override { return "Fire Rate +5%"; }
    std::string getDescription() const override { return "Shoot faster by 5%."; }
    void apply(Player& player) override;
};

class SpeedBoostUpgrade : public Upgrade {
public:
    std::string getName() const override { return "Speed +5%"; }
    std::string getDescription() const override { return "Increases move speed by 5%."; }
    void apply(Player& player) override;
};

class HpRegenUpgrade : public Upgrade {
public:
    std::string getName() const override { return "HP Regen +1"; }
    std::string getDescription() const override { return "Restore more HP every second."; }
    void apply(Player& player) override;
};

class VampirismUpgrade : public Upgrade {
public:
    std::string getName() const override { return "Vampirism +1"; }
    std::string getDescription() const override { return "Restore 1 HP per kill."; }
    void apply(Player& player) override;
};

class XPGainUpgrade : public Upgrade {
public:
    std::string getName() const override { return "XP Gain +25%"; }
    std::string getDescription() const override { return "Gain more XP from orbs."; }
    void apply(Player& player) override;
};