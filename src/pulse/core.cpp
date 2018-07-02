#include "core.hpp"
#include "config.hpp"

#include <algorithm>
#include <iostream>
#include <cassert>

constexpr auto HeroSpeed = 1.0;
constexpr auto HeroAcceleration = 10.0;
constexpr auto HeroDeceleration = 10.0;
constexpr auto HeroMaxSpeed = 3.0;

constexpr auto HeroMovementAcceleration = HeroDeceleration + HeroAcceleration;

void Hero::update(double delta)
{
    velocity += controls.movement * HeroMovementAcceleration * delta;
    auto speed = velocity.length();
    auto speedWithFriction = speed - HeroDeceleration * delta;
    auto desiredSpeed = std::clamp(speedWithFriction, 0.0, HeroMaxSpeed);
    if (speed > 0) {
        velocity *= desiredSpeed / speed;
    }

    data.position += velocity * delta;
}

const std::vector<std::vector<Vector<double>>> Enemy::PerfectTentacles {
    {
        {0.25, -0.25}, {0.5, 0}, {0.75, 1}, {0.5, 2},
    },
    {
        {-0.25, -0.25}, {-0.5, 0}, {-0.75, 1}, {-0.5, 2},
    },
};

const std::vector<std::vector<Vector<double>>> Enemy::PreparedTentacles {
    {
        {0.1, -0.4}, {0.5, -0.75}, {0.75, 0.2}, {0.7, 0.5},
    },
    {
        {-0.25, -0.25}, {-0.5, 0}, {-0.75, 1}, {-0.5, 2},
    },
};

void Enemy::update(
    double delta,
    const Vector<double>& heroPosition)
{
    static constexpr auto DesiredDistanceToHero = 3.0;
    static constexpr auto Speed = 2.0;

    switch (state) {
        case State::Manouver:
        {
            auto vectorToHero = heroPosition - data.position;
            auto directionToHero = vectorToHero.normalized();
            auto directionLeft = directionToHero.rotateCcw90();

            auto distanceToHero = vectorToHero.length();
            auto offsetToHero = distanceToHero - DesiredDistanceToHero;
            auto direction =
                (directionToHero * offsetToHero + directionLeft).normalized();

            data.position += direction * Speed * delta;

            //positionTentacle(delta, heroPosition, data.tentacles[0], PreparedTentacles.at(0));
            strike(delta, data.tentacles[0], heroPosition);
            positionTentacle(delta, heroPosition, data.tentacles[1], PerfectTentacles.at(1));
        }
        break;

        case State::Attack:
            break;
    }
}

void Enemy::positionTentacle(
    double delta,
    const Vector<double>& faceTarget,
    std::vector<Vector<double>>& tentacle,
    const std::vector<Vector<double>>& targetPosition,
    double speed)
{
    auto newY = (faceTarget - data.position).normalized();
    auto newX = newY.rotateCw90();

    for (size_t i = 0; i < tentacle.size(); i++) {
        const auto& perfectJoint = targetPosition.at(i);
        auto targetJoint = Vector<double>{
            perfectJoint.x * newX.x + perfectJoint.y * newY.x,
            perfectJoint.x * newX.y + perfectJoint.y * newY.y};

        auto& joint = tentacle.at(i);
        auto vectorToTarget = targetJoint - joint;
        auto distanceToTarget = vectorToTarget.length();
        auto timeToTarget = distanceToTarget / speed;
        if (timeToTarget <= delta) {
            joint = targetJoint;
        } else {
            joint += vectorToTarget.normalized() * speed * delta;
        }
    }
}

void Enemy::strike(
    double delta,
    std::vector<Vector<double>>& tentacle,
    const Vector<double>& target)
{
    auto distanceToTarget = (target - data.position).length();
    std::vector<Vector<double>> targetPosition {
        {0.25, -0.25}, {0.5, 0}, {0.75, 1}, {0, distanceToTarget},
    };
    positionTentacle(
        delta, target, tentacle, targetPosition, config::TentacleStrikeSpeed);
}

const Scene& Core::scene() const
{
    return _scene;
}

Core::Core()
    : _hero(_scene.hero)
{ }

void Core::update(double delta)
{
    _hero.update(delta);
    for (auto& enemy : _enemies) {
        enemy.update(delta, _hero.data.position);
    }
}

void Core::placeHero(const Vector<double>& heroPosition)
{
    _hero.data.position = heroPosition;
}

void Core::spawnEnemy(const Vector<double>& enemyPosition)
{
    _scene.enemies.push_back({enemyPosition, {}});
    _scene.enemies.back().tentacles.push_back({
        {0.25, -0.25}, {0.5, 0}, {0.75, 1}, {0.5, 2},
    });
    _scene.enemies.back().tentacles.push_back({
        {-0.25, -0.25}, {-0.5, 0}, {-0.75, 1}, {-0.5, 2},
    });

    _enemies.emplace_back(_scene.enemies.back());
}
