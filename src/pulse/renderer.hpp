#pragma once

#include <SDL.h>

#include "scene.hpp"

struct Camera {
    Vector<double> position;
    Vector<double> span;
};

class Renderer {
public:
    Renderer();
    ~Renderer();

    void update(double delta);

    void clear();
    void present() const;

    void renderScene(const Scene& scene);

    void placeCamera(
        const Vector<double>& cameraPosition,
        const Vector<double>& cameraSpan);

private:
    Vector<int> screenPoint(const Vector<double>& worldPoint);
    void renderHero(const HeroData& heroData);
    void renderEnemy(const EnemyData& enemyData);

    SDL_Window* _window = nullptr;
    SDL_Renderer* _renderer = nullptr;
    Vector<int> _screenSize;
    Camera _camera;
};
