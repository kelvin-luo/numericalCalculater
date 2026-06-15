#include "numcalc.h"
#include "nc_plot.h"

#include <sstream>
#include <iomanip>
#include <cmath>
#include <random>
#include <vector>
#include <array>

namespace numcalc {

namespace {
double Sigmoid(double z) { return 1.0 / (1.0 + std::exp(-z)); }
double DSigmoid(double a) { return a * (1.0 - a); }
}  // namespace

// A 2-2-1 multilayer perceptron trained with back-propagation on XOR.
NcResult RunAnn() {
    NcResult res;
    res.title = "6. ANN - MLP solving XOR";

    const double X[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    const double T[4] = {0, 1, 1, 0};

    std::mt19937 rng(123);
    std::uniform_real_distribution<double> u(-1.0, 1.0);

    // Layer 1: 2 inputs -> 2 hidden ; Layer 2: 2 hidden -> 1 output.
    double w1[2][2], b1[2], w2[2], b2;
    for (int i = 0; i < 2; ++i) {
        b1[i] = u(rng);
        w2[i] = u(rng);
        for (int j = 0; j < 2; ++j) w1[i][j] = u(rng);
    }
    b2 = u(rng);

    const double lr = 0.5;
    const int epochs = 20000;
    std::vector<double> lossCurve;
    for (int e = 0; e < epochs; ++e) {
        double epochLoss = 0;
        for (int s = 0; s < 4; ++s) {
            // Forward.
            double h[2];
            for (int i = 0; i < 2; ++i)
                h[i] = Sigmoid(w1[i][0] * X[s][0] + w1[i][1] * X[s][1] + b1[i]);
            double o = Sigmoid(w2[0] * h[0] + w2[1] * h[1] + b2);
            double err = o - T[s];
            epochLoss += 0.5 * err * err;

            // Backward.
            double dOut = err * DSigmoid(o);
            double dH[2];
            for (int i = 0; i < 2; ++i) dH[i] = dOut * w2[i] * DSigmoid(h[i]);
            for (int i = 0; i < 2; ++i) {
                w2[i] -= lr * dOut * h[i];
                b1[i] -= lr * dH[i];
                for (int j = 0; j < 2; ++j) w1[i][j] -= lr * dH[i] * X[s][j];
            }
            b2 -= lr * dOut;
        }
        if (e % 200 == 0) lossCurve.push_back(epochLoss);
    }

    std::ostringstream os;
    os << std::fixed << std::setprecision(4);
    os << "Network: 2 - 2 - 1, sigmoid, back-prop, " << epochs
       << " epochs, lr = " << lr << "\n\n";
    os << "Truth table prediction:\n";
    os << "    x1 x2 | target | output\n";
    for (int s = 0; s < 4; ++s) {
        double h[2];
        for (int i = 0; i < 2; ++i)
            h[i] = Sigmoid(w1[i][0] * X[s][0] + w1[i][1] * X[s][1] + b1[i]);
        double o = Sigmoid(w2[0] * h[0] + w2[1] * h[1] + b2);
        os << "     " << X[s][0] << "  " << X[s][1] << "  |   " << T[s]
           << "    |  " << o << "\n";
    }
    os << "\nFinal training loss = "
       << (lossCurve.empty() ? 0.0 : lossCurve.back()) << "\n";
    res.report = os.str();

    // Visualization: training loss curve.
    Plot plot;
    plot.title("ANN training loss (XOR)").labels("epoch (x200)", "loss");
    plot.curve(lossCurve, cv::Scalar(40, 120, 220), "loss");
    res.plot = plot.render();
    return res;
}

}  // namespace numcalc
