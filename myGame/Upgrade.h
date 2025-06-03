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

class MegaHealthUpgrade : public Upgrade {
public:
    std::string getName() const override { return "Max Health +20%"; }
    std::string getDescription() const override { return "Increases max health by 20%."; }
    void apply(Player& player) override;
};


class FireRateUpgrade : public Upgrade {
public:
    std::string getName() const override { return "Fire Rate +5%"; }
    std::string getDescription() const override { return "Shoot faster by 5%."; }
    void apply(Player& player) override;
};

class RapidFireUpgrade : public Upgrade {
public:
    std::string getName() const override { return "Fire Rate +20%"; }
    std::string getDescription() const override { return "Shoot much faster."; }
    void apply(Player& player) override;
};


class SpeedBoostUpgrade : public Upgrade {
public:
    std::string getName() const override { return "Speed +5%"; }
    std::string getDescription() const override { return "Increases move speed by 5%."; }
    void apply(Player& player) override;
};

class SprintBoostUpgrade : public Upgrade {
public:
    std::string getName() const override { return "Speed +20%"; }
    std::string getDescription() const override { return "Significantly increases move speed."; }
    void apply(Player& player) override;
};


class HpRegenUpgrade : public Upgrade {
public:
    std::string getName() const override { return "HP Regen +1"; }
    std::string getDescription() const override { return "Restore more HP every second."; }
    void apply(Player& player) override;
};

class AdvancedHpRegenUpgrade : public Upgrade {
public:
    std::string getName() const override { return "HP Regen +3"; }
    std::string getDescription() const override { return "Restore 3 HP every second."; }
    void apply(Player& player) override;
};


class VampirismUpgrade : public Upgrade {
public:
    std::string getName() const override { return "Vampirism +1"; }
    std::string getDescription() const override { return "Restore 1 HP per kill."; }
    void apply(Player& player) override;
};

class GreaterVampirismUpgrade : public Upgrade {
public:
    std::string getName() const override { return "Vampirism +3"; }
    std::string getDescription() const override { return "Restore 3 HP per kill."; }
    void apply(Player& player) override;
};


class XPGainUpgrade : public Upgrade {
public:
    std::string getName() const override { return "XP Gain +25%"; }
    std::string getDescription() const override { return "Gain more XP from orbs."; }
    void apply(Player& player) override;
};

class XPSurgeUpgrade : public Upgrade {
public:
    std::string getName() const override { return "XP Gain +100%"; }
    std::string getDescription() const override { return "Double XP gain."; }
    void apply(Player& player) override;
};

class ShieldUpgrade : public Upgrade {
public:
    std::string getName() const override { return "Shield"; }
    std::string getDescription() const override { return "Blocks one hit every 10 seconds."; }
    void apply(Player& player) override;
};
