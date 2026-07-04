#pragma once

class RMS {
    public:
        void prepare(double sampleRate, double windowMs);
        void processSample(float sample);
        float getRMS() const;
        float getDBFS() const;
    private:
        double sr = 44100.0;
        int windowSamples = 1024;
        double sumSquares = 0.0;
        int count = 0;
};