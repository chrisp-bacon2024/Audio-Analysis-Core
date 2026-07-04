#pragma once

enum class WeightingType
{
    Z, // Flat / unweighted
    A,
    C
};

class Weighting 
{
    public:
        static float getWeightingDb(float frequencyHz, WeightingType weighting);
    private:
        static float getAWeightingDb(float frequencyHz);
        static float getCWeightingDb(float frequencyHz);
};