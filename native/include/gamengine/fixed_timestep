#pragma once

#include <functional>
#include <algorithm>

namespace GAMENGINE {

class FixedTimestep {
public:
    explicit FixedTimestep(
        double step = 1.0 / 60.0,
        double maxFrame = 0.25
    )
        : fixedStep(step),
          maxFrameTime(maxFrame) {}

    void update(
        double deltaTime,
        const std::function<void(double)>& callback
    ) {
        deltaTime = std::clamp(
            deltaTime, 0.0, maxFrameTime
        );

        accumulator += deltaTime;

        while (accumulator >= fixedStep) {
            callback(fixedStep);
            accumulator -= fixedStep;
            ++tickCount;
        }
    }

    double interpolation() const {
        return accumulator / fixedStep;
    }

    unsigned long long ticks() const {
        return tickCount;
    }

    void reset() {
        accumulator = 0.0;
        tickCount = 0;
    }

private:
    double fixedStep;
    double maxFrameTime;
    double accumulator = 0.0;

    unsigned long long tickCount = 0;
};

}
