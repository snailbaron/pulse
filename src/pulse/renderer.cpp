#include "renderer.hpp"
#include "config.hpp"

#include <SDL2/SDL2_gfxPrimitives.h>

#include <cassert>
#include <iostream>

Renderer::Renderer()
{
    _window = SDL_CreateWindow(
        config::WindowTitle,
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        config::WindowWidth, config::WindowHeight,
        0);
    _screenSize = {config::WindowWidth, config::WindowHeight};
    _renderer = SDL_CreateRenderer(
        _window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

Renderer::~Renderer()
{
    assert(_renderer);
    SDL_DestroyRenderer(_renderer);
    assert(_window);
    SDL_DestroyWindow(_window);
}

void Renderer::update(double /*delta*/)
{
}

void Renderer::clear()
{
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_renderer);
}

void Renderer::present() const
{
    SDL_RenderPresent(_renderer);
}

void Renderer::renderScene(const Scene& scene)
{
    Vector<int> screenSize;
    SDL_GetWindowSize(_window, &screenSize.x, &screenSize.y);

    renderHero(scene.hero);
    for (const auto& enemy : scene.enemies) {
        renderEnemy(enemy);
    }
}

void Renderer::placeCamera(
    const Vector<double>& cameraPosition,
    const Vector<double>& cameraSpan)
{
    _camera.position = cameraPosition;
    _camera.span = cameraSpan;
}

Vector<int> Renderer::screenPoint(const Vector<double>& worldPoint)
{
    const auto& center = _camera.position;
    const auto& span = _camera.span;

    auto invertedOffset =
        Vector<double>{worldPoint.x - center.x, center.y - worldPoint.y};
    auto relativePoint = invertedOffset + span / 2;

    return {
        static_cast<int>(relativePoint.x * _screenSize.x / span.x),
        static_cast<int>(relativePoint.y * _screenSize.y / span.y)};
};


void Renderer::renderHero(const HeroData& heroData)
{
    auto heroScreenPosition = screenPoint(heroData.position);
    filledEllipseRGBA(
        _renderer,
        heroScreenPosition.x, heroScreenPosition.y,
        15, 20,
        0, 255, 0, 255);
}

void Renderer::renderEnemy(const EnemyData& enemyData)
{
    auto screenPosition = screenPoint(enemyData.position);
    filledEllipseRGBA(
        _renderer,
        screenPosition.x, screenPosition.y,
        15, 20,
        255, 255, 255, 255);

    for (const auto& tentacle : enemyData.tentacles) {
        auto previousPoint = screenPosition;
        for (const auto& joint : tentacle) {
            auto jointPosition = enemyData.position + joint;
            auto screenJoint = screenPoint(jointPosition);
            thickLineRGBA(
                _renderer,
                previousPoint.x, previousPoint.y,
                screenJoint.x, screenJoint.y,
                3,
                100, 100, 100, 255);
            previousPoint = screenJoint;
        }
    }
}
