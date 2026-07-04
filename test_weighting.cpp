// Sweeps 20 Hz - 20 kHz (log-spaced) and plots A- and C-weighting using
// the project's Weighting class. Produces:
//   1. A console ASCII plot of both curves.
//   2. A verification table comparing computed values to IEC 61672 references.
//   3. weighting_curve.csv for plotting in Excel/Python/gnuplot.
//
// Build (from the project root):
//   g++ test_weighting.cpp dBUtils.cpp -std=c++17 -o test_weighting
// Run:
//   ./test_weighting        (Windows: .\test_weighting.exe)

#include "dBUtils.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

namespace {

constexpr float kFreqMin = 20.0f;
constexpr float kFreqMax = 20000.0f;

// Log-spaced frequency: t in [0,1] maps 20 Hz -> 20 kHz.
float logFreq(float t) {
    return kFreqMin * std::pow(kFreqMax / kFreqMin, t);
}

float logPos(float f) {
    return std::log(f / kFreqMin) / std::log(kFreqMax / kFreqMin);
}

void drawPlot(const std::vector<float>& freqs,
              const std::vector<float>& aVals,
              const std::vector<float>& cVals) {
    const int width = static_cast<int>(freqs.size());
    constexpr int   kHeight = 28;
    constexpr float kDbMax  = 6.0f;
    constexpr float kDbMin  = -60.0f;

    auto rowFor = [&](float db) {
        db = std::clamp(db, kDbMin, kDbMax);
        const float frac = (kDbMax - db) / (kDbMax - kDbMin);
        return static_cast<int>(std::lround(frac * (kHeight - 1)));
    };

    std::vector<std::string> grid(kHeight, std::string(width, ' '));
    for (int i = 0; i < width; ++i) {
        const int rc = rowFor(cVals[i]);
        const int ra = rowFor(aVals[i]);
        grid[rc][i] = 'C';
        grid[ra][i] = (ra == rc) ? '*' : 'A';
    }

    std::printf("\nA- and C-weighting  (20 Hz - 20 kHz, log frequency)\n");
    std::printf("A = A-weighting   C = C-weighting   * = overlap\n\n");

    for (int r = 0; r < kHeight; ++r) {
        const float db =
            kDbMax - (static_cast<float>(r) / (kHeight - 1)) * (kDbMax - kDbMin);
        if (r % 3 == 0)
            std::printf("%6.1f |%s\n", db, grid[r].c_str());
        else
            std::printf("       |%s\n", grid[r].c_str());
    }

    std::printf("       +%s\n", std::string(width, '-').c_str());

    // Frequency tick labels along the x-axis.
    std::string labels(width, ' ');
    auto place = [&](float f, const std::string& txt) {
        const int col = static_cast<int>(std::lround(logPos(f) * (width - 1)));
        for (int k = 0; k < static_cast<int>(txt.size()); ++k) {
            const int pos = col + k;
            if (pos >= 0 && pos < width) labels[pos] = txt[k];
        }
    };
    place(20.0f, "20");
    place(100.0f, "100");
    place(1000.0f, "1k");
    place(10000.0f, "10k");
    place(20000.0f, "20k");
    std::printf("        %s\n", labels.c_str());
}

void printTable() {
    // Nominal frequencies with IEC 61672 reference weightings (dB).
    struct Ref { float f, a, c; };
    const Ref refs[] = {
        {   20.0f, -50.5f, -6.2f},
        {   31.5f, -39.4f, -3.0f},
        {   50.0f, -30.2f, -1.3f},
        {  100.0f, -19.1f, -0.3f},
        {  200.0f, -10.9f,  0.0f},
        {  500.0f,  -3.2f,  0.0f},
        { 1000.0f,   0.0f,  0.0f},
        { 2000.0f,   1.2f, -0.2f},
        { 5000.0f,   0.5f, -0.8f},
        {10000.0f,  -2.5f, -4.4f},
        {20000.0f,  -9.3f,-11.2f},
    };

    std::printf("\nVerification vs IEC 61672 reference values:\n");
    std::printf("  Freq (Hz) |  A calc |  A ref |  C calc |  C ref\n");
    std::printf("  ----------+---------+--------+---------+--------\n");
    for (const auto& r : refs) {
        const float a = Weighting::getWeightingDb(r.f, WeightingType::A);
        const float c = Weighting::getWeightingDb(r.f, WeightingType::C);
        std::printf("  %9.1f | %7.2f | %6.1f | %7.2f | %6.1f\n",
                    r.f, a, r.a, c, r.c);
    }
    std::printf("\n(Small deviations of a few tenths of a dB are expected: the\n");
    std::printf(" code uses the 12200 Hz analog approximation, not 12194.)\n");
}

}  // namespace

int main() {
    constexpr int kNumPoints = 72;

    std::vector<float> freqs(kNumPoints), aVals(kNumPoints), cVals(kNumPoints);
    for (int i = 0; i < kNumPoints; ++i) {
        const float t = static_cast<float>(i) / (kNumPoints - 1);
        const float f = logFreq(t);
        freqs[i] = f;
        aVals[i] = Weighting::getWeightingDb(f, WeightingType::A);
        cVals[i] = Weighting::getWeightingDb(f, WeightingType::C);
    }

    std::ofstream csv("weighting_curve.csv");
    csv << "frequency_hz,a_weighting_db,c_weighting_db\n";
    for (int i = 0; i < kNumPoints; ++i)
        csv << freqs[i] << ',' << aVals[i] << ',' << cVals[i] << '\n';

    drawPlot(freqs, aVals, cVals);
    printTable();
    std::printf("\nWrote %d points to weighting_curve.csv\n", kNumPoints);
    return 0;
}
