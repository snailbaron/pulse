#pragma once

#include "vector.hpp"
#include "scene.hpp"
#include "hero_controller.hpp"
#include "config.hpp"

#include <vector>

struct Hero {
    Hero(HeroData& heroData)
        : data(heroData)
    { }

    void update(double delta);

    HeroData& data;
    Vector<double> velocity;
    HeroControls controls;
    int health;
};

class Enemy {
public:
    Enemy(EnemyData& enemyData)
        : data(enemyData)
    { }

    void update(
        double delta,
        const Vector<double>& heroPosition);

private:
    static const std::vector<std::vector<Vector<double>>> PerfectTentacles;
    static const std::vector<std::vector<Vector<double>>> PreparedTentacles;

    enum class State {
        Manouver,
        Attack,
    };

    void positionTentacle(
        double delta,
        const Vector<double>& faceTarget,
        std::vector<Vector<double>>& tentacle,
        const std::vector<Vector<double>>& targetPosition,
        double speed = config::TentacleJointSpeed);

    void strike(
        double delta,
        std::vector<Vector<double>>& tentacle,
        const Vector<double>& target);

    EnemyData& data;
    int health;
    State state = State::Manouver;
};

class Core {
public:
    Core();

    const Scene& scene() const;
    HeroControls& heroControls() { return _hero.controls; }

    void update(double delta);

    void placeHero(const Vector<double>& heroPosition);
    void spawnEnemy(const Vector<double>& enemyPosition);

private:
    Scene _scene;
    Hero _hero;
    std::vector<Enemy> _enemies;
};
