#include "RMS.h"
#include <cmath>
#include <algorithm>

void RMS::prepare(double sampleRate, double windowMs)
{
    sr = sampleRate;
    windowSamples = static_cast<int>(sr * windowMs / 1000.0);
    sumSquares = 0.0;
    count = 0;
}
void RMS::processSample(float sample)
{
    sumSquares += sample * sample;
    count++;

    if (count > windowSamples)
    {
        sumSquares = 0.0;
        count = 0;
    }
}
float RMS::getRMS() const
{
    if (count == 0) return 0.0f;
    return std::sqrt(sumSquares / count);
}
float RMS::getDBFS() const
{
    constexpr float epsilon = 1e-12f;
    return 20.0f * std::log10(std::max(getRMS(), epsilon));
}