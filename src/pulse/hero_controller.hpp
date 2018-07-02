#pragma once

#include "vector.hpp"

#include <SDL.h>

#include <map>
#include <cstdint>
#include <optional>

struct HeroControls {
    Vector<double> movement;
};

enum class Action {
    MoveLeft,
    MoveRight,
    MoveUp,
    MoveDown,
    Strike,
};

struct MovementButtonState {
    void update(Action action, bool pressed);
    Vector<double> movement() const;

    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;
};

class HeroControlScheme {
public:
    std::optional<Action> action(SDL_Keycode keycode) const;
    void mapAction(Action action, SDL_Keycode keycode);

private:
    std::map<SDL_Keycode, Action> _keyToAction;
    std::map<Action, SDL_Keycode> _actionToKey;
};

class HeroController {
public:
    HeroController(const HeroControlScheme& scheme, HeroControls& controls)
        : _scheme(scheme), _controls(controls)
    { }

    void processEvent(const SDL_Event& event);

private:
    const HeroControlScheme& _scheme;
    HeroControls& _controls;
    MovementButtonState _movementButtons;
};
