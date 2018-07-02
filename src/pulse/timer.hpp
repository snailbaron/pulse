#pragma once

#include <chrono>

class FrameTimer {
public:
    FrameTimer(int fps);
    double delta() const;
    int framesPassed();

private:
    using Clock = std::chrono::high_resolution_clock;

    long long _lastFrame;
    Clock::time_point _startTime;
    double _delta;
    Clock::duration _frameDuration;
};
