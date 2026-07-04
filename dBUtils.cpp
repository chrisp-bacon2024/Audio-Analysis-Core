#include "dBUtils.h"
#include <cmath>

float Weighting::getWeightingDb(float frequencyHz, WeightingType weighting)
{
    switch (weighting)
    {
        case WeightingType::A:
            return getAWeightingDb(frequencyHz);
        case WeightingType::C:
            return getCWeightingDb(frequencyHz);
        case WeightingType::Z:
        default:
            return 0.0f;
    }
}
float Weighting::getAWeightingDb(float frequencyHz)
{
    const float f2 = frequencyHz * frequencyHz;

    const float numerator = 12200.0f * 12200.0f * f2 * f2;

    const float denominator = (f2 + 20.6f * 20.6f) * 
    std::sqrt((f2 + 107.7f * 107.7f) * (f2 + 737.9f * 737.9f)) *
    (f2 + 12200.0f * 12200.0f);

    return 20.0f * std::log10(numerator / denominator) + 2.0f;
}

float Weighting::getCWeightingDb(float frequencyHz)
{
    const float f2 = frequencyHz * frequencyHz;

    const float numerator = 12200.0f * 12200.0f * f2;

    const float denominator = (f2 + 20.6f * 20.6f) * (f2 + 12200.0f * 12200.0f);

    return 20.0f * std::log10(numerator / denominator) + 0.06f;
}