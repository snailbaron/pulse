#pragma once

#include "vector.hpp"

#include <vector>

struct HeroData {
    Vector<double> position;
};

struct EnemyData {
    Vector<double> position;
    std::vector<std::vector<Vector<double>>> tentacles;
};

struct Scene {
    HeroData hero;
    std::vector<EnemyData> enemies;
};
