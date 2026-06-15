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
}  // namespace

NcResult RunLogisticRegression() {
    NcResult res;
    res.title = "4. Logistic Regression (binary classification)";

    // Generate two Gaussian blobs that are linearly separable-ish.
    std::mt19937 rng(2026);
    std::normal_distribution<double> g(0.0, 0.7);
    std::vector<std::array<double, 2>> X;
    std::vector<int> Y;
    for (int i = 0; i < 60; ++i) {
        X.push_back({2.0 + g(rng), 2.0 + g(rng)}); Y.push_back(0);
        X.push_back({5.0 + g(rng), 5.0 + g(rng)}); Y.push_back(1);
    }
    const size_t n = X.size();

    // Gradient descent on weights (w0, w1) and bias b.
    double w0 = 0, w1 = 0, b = 0;
    const double lr = 0.1;
    const int epochs = 2000;
    std::vector<double> lossCurve;
    for (int e = 0; e < epochs; ++e) {
        double gw0 = 0, gw1 = 0, gb = 0, loss = 0;
        for (size_t i = 0; i < n; ++i) {
            double z = w0 * X[i][0] + w1 * X[i][1] + b;
            double p = Sigmoid(z);
            double err = p - Y[i];
            gw0 += err * X[i][0];
            gw1 += err * X[i][1];
            gb  += err;
            double eps = 1e-12;
            loss += -(Y[i] * std::log(p + eps) +
                      (1 - Y[i]) * std::log(1 - p + eps));
        }
        w0 -= lr * gw0 / n;
        w1 -= lr * gw1 / n;
        b  -= lr * gb  / n;
        if (e % 20 == 0) lossCurve.push_back(loss / n);
    }

    int correct = 0;
    for (size_t i = 0; i < n; ++i) {
        double p = Sigmoid(w0 * X[i][0] + w1 * X[i][1] + b);
        if ((p >= 0.5 ? 1 : 0) == Y[i]) ++correct;
    }

    std::ostringstream os;
    os << std::fixed << std::setprecision(4);
    os << "Two classes, " << n << " samples, gradient descent " << epochs
       << " epochs, lr = " << lr << "\n\n";
    os << "Learned model:  sigmoid(" << w0 << "*x1 + " << w1 << "*x2 + " << b
       << ")\n";
    os << "Decision boundary: " << w0 << "*x1 + " << w1 << "*x2 + " << b
       << " = 0\n\n";
    os << "Training accuracy = " << (100.0 * correct / n) << " %  (" << correct
       << "/" << n << ")\n";
    os << "Final loss = " << (lossCurve.empty() ? 0.0 : lossCurve.back())
       << "\n";
    res.report = os.str();

    // Visualization: scatter colored by class + decision boundary line.
    Plot plot;
    plot.title("Logistic regression decision boundary").labels("x1", "x2");
    std::vector<cv::Point2d> pts;
    for (size_t i = 0; i < n; ++i) pts.emplace_back(X[i][0], X[i][1]);
    plot.scatter(pts, Y, 4);
    // boundary: x2 = -(w0*x1 + b)/w1
    if (std::abs(w1) > 1e-9) {
        std::vector<double> bx = {0.5, 6.5};
        std::vector<double> by = {-(w0 * bx[0] + b) / w1,
                                  -(w0 * bx[1] + b) / w1};
        plot.curve(bx, by, cv::Scalar(20, 20, 20), "boundary");
    }
    res.plot = plot.render();
    return res;
}

}  // namespace numcalc
