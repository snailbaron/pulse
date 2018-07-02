#include "timer.hpp"

FrameTimer::FrameTimer(int fps)
    : _lastFrame(0)
    , _startTime(Clock::now())
    , _delta(1.0 / fps)
    , _frameDuration(std::chrono::duration_cast<Clock::duration>(
        std::chrono::duration<double>(_delta)))
{ }

double FrameTimer::delta() const
{
    return _delta;
}

int FrameTimer::framesPassed()
{
    auto currentTime = Clock::now();
    auto currentFrame = (currentTime - _startTime) / _frameDuration;
    auto frameDiff = currentFrame - _lastFrame;
    _lastFrame = currentFrame;
    return static_cast<int>(frameDiff);
}
