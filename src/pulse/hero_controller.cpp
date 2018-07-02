#include "hero_controller.hpp"

#include <iostream>

void MovementButtonState::update(Action action, bool pressed)
{
    switch (action) {
        case Action::MoveLeft: left = pressed; break;
        case Action::MoveRight: right = pressed; break;
        case Action::MoveUp: up = pressed; break;
        case Action::MoveDown: down = pressed; break;
        default: break;
    }
}

Vector<double> MovementButtonState::movement() const
{
    int x = 0;
    int y = 0;

    if (left) x--;
    if (right) x++;
    if (up) y++;
    if (down) y--;

    return {1.0 * x, 1.0 * y};
}

std::optional<Action> HeroControlScheme::action(SDL_Keycode keycode) const
{
    if (auto it = _keyToAction.find(keycode); it != _keyToAction.end()) {
        return it->second;
    }
    return {};
}

void HeroControlScheme::mapAction(Action action, SDL_Keycode keycode)
{
    if (auto it = _actionToKey.find(action); it != _actionToKey.end()) {
        _keyToAction.erase(it->second);
    }
    if (auto it = _keyToAction.find(keycode); it != _keyToAction.end()) {
        _actionToKey.erase(it->second);
    }
    _actionToKey[action] = keycode;
    _keyToAction[keycode] = action;
}

void HeroController::processEvent(const SDL_Event& event)
{
    if ((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) &&
            !event.key.repeat) {
        auto action = _scheme.action(event.key.keysym.sym);
        if (action) {
            _movementButtons.update(*action, event.type == SDL_KEYDOWN);
            _controls.movement = _movementButtons.movement();
        }
    }

}
