"""Plot weighting_curve.csv produced by test_weighting.exe.

This does NO weighting math of its own -- it only reads the CSV that your
C++ code (Weighting::getWeightingDb) wrote, so the plot is a direct view of
your C++ output.

Usage:
    python plot_from_csv.py            # reads weighting_curve.csv
"""

import sys
import numpy as np
import matplotlib.pyplot as plt

csv_path = sys.argv[1] if len(sys.argv) > 1 else "weighting_curve.csv"

data = np.genfromtxt(csv_path, delimiter=",", names=True)
freq = data["frequency_hz"]
a_db = data["a_weighting_db"]
c_db = data["c_weighting_db"]

fig, ax = plt.subplots(figsize=(10, 6))
ax.semilogx(freq, a_db, label="A-weighting", color="#e2492f", linewidth=2,
            marker="o", markersize=3)
ax.semilogx(freq, c_db, label="C-weighting", color="#2f6fe2", linewidth=2,
            marker="o", markersize=3)

ax.axhline(0.0, color="#888888", linewidth=0.8, linestyle="--")
ax.set_title("A- and C-weighting (data from test_weighting.exe / your C++)")
ax.set_xlabel("Frequency (Hz)")
ax.set_ylabel("Relative response (dB)")
ax.set_xlim(20, 20000)
ax.set_ylim(-60, 10)
ax.grid(True, which="both", alpha=0.3)

ticks = [20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000]
ax.set_xticks(ticks)
ax.set_xticklabels([str(t) if t < 1000 else f"{t // 1000}k" for t in ticks])
ax.legend(loc="lower center")

fig.tight_layout()
out = "weighting_plot.png"
fig.savefig(out, dpi=130)
print(f"Read {len(freq)} rows from {csv_path}, saved {out}")
