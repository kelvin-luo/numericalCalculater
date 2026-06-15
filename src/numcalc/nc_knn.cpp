#include "numcalc.h"
#include "nc_plot.h"

#include <sstream>
#include <iomanip>
#include <cmath>
#include <random>
#include <vector>
#include <array>
#include <algorithm>
#include <opencv2/imgproc.hpp>

namespace numcalc {

namespace {
int KnnPredict(const std::vector<std::array<double, 2>>& X,
               const std::vector<int>& Y, double px, double py, int k,
               int nClasses) {
    std::vector<std::pair<double, int>> d;
    d.reserve(X.size());
    for (size_t i = 0; i < X.size(); ++i) {
        double dx = X[i][0] - px, dy = X[i][1] - py;
        d.emplace_back(dx * dx + dy * dy, Y[i]);
    }
    std::partial_sort(d.begin(), d.begin() + std::min<size_t>(k, d.size()),
                      d.end());
    std::vector<int> votes(nClasses, 0);
    for (int i = 0; i < k && i < static_cast<int>(d.size()); ++i)
        ++votes[d[i].second];
    return static_cast<int>(
        std::max_element(votes.begin(), votes.end()) - votes.begin());
}
}  // namespace

NcResult RunKnn() {
    NcResult res;
    res.title = "5. K-Nearest-Neighbors (k-NN)";

    const int nClasses = 3;
    std::mt19937 rng(7);
    std::normal_distribution<double> g(0.0, 0.6);
    std::array<std::array<double, 2>, 3> centers = {{{2, 2}, {6, 3}, {4, 6}}};
    std::vector<std::array<double, 2>> X;
    std::vector<int> Y;
    for (int c = 0; c < nClasses; ++c)
        for (int i = 0; i < 25; ++i) {
            X.push_back({centers[c][0] + g(rng), centers[c][1] + g(rng)});
            Y.push_back(c);
        }

    const int k = 5;
    // Classify a few query points.
    std::array<std::array<double, 2>, 3> queries = {{{3, 3}, {5, 5}, {6, 2}}};
    std::ostringstream os;
    os << std::fixed << std::setprecision(2);
    os << "Training set: " << X.size() << " points, " << nClasses
       << " classes, k = " << k << "\n\n";
    os << "Class centers: (2,2), (6,3), (4,6)\n\n";
    os << "Query predictions:\n";
    for (auto& q : queries) {
        int pred = KnnPredict(X, Y, q[0], q[1], k, nClasses);
        os << "    (" << q[0] << ", " << q[1] << ")  ->  class " << pred
           << "\n";
    }
    res.report = os.str();

    // Visualization: paint the decision regions, then overlay the samples.
    const double xMin = 0, xMax = 8, yMin = 0, yMax = 8;
    const int W = 700, H = 460, marginL = 60, marginR = 20, marginT = 40,
              marginB = 44;
    cv::Mat canvas(H, W, CV_8UC3, cv::Scalar(255, 255, 255));
    const cv::Scalar region[] = {
        {235, 225, 210}, {210, 235, 220}, {220, 215, 240}};
    for (int py = marginT; py < H - marginB; py += 4) {
        for (int px = marginL; px < W - marginR; px += 4) {
            double fx = xMin + (px - marginL) * (xMax - xMin) /
                                   (W - marginL - marginR);
            double fy = yMax - (py - marginT) * (yMax - yMin) /
                                   (H - marginT - marginB);
            int c = KnnPredict(X, Y, fx, fy, k, nClasses);
            cv::rectangle(canvas, cv::Point(px, py), cv::Point(px + 4, py + 4),
                          region[c % 3], -1);
        }
    }
    Plot plot;  // reuse plot only for the framed scatter overlay
    (void)plot;
    const cv::Scalar palette[] = {
        {60, 76, 231}, {231, 156, 60}, {76, 175, 80}};
    auto toPix = [&](double x, double y) {
        int px = marginL + static_cast<int>((x - xMin) / (xMax - xMin) *
                                            (W - marginL - marginR));
        int py = marginT + static_cast<int>((yMax - y) / (yMax - yMin) *
                                            (H - marginT - marginB));
        return cv::Point(px, py);
    };
    cv::rectangle(canvas, cv::Point(marginL, marginT),
                  cv::Point(W - marginR, H - marginB), cv::Scalar(80, 80, 80),
                  2);
    for (size_t i = 0; i < X.size(); ++i)
        cv::circle(canvas, toPix(X[i][0], X[i][1]), 4, palette[Y[i] % 3], -1,
                   cv::LINE_AA);
    for (auto& q : queries)
        cv::drawMarker(canvas, toPix(q[0], q[1]), cv::Scalar(20, 20, 20),
                       cv::MARKER_STAR, 16, 2, cv::LINE_AA);
    cv::putText(canvas, "k-NN decision regions (stars = queries)",
                cv::Point(marginL, marginT - 14), cv::FONT_HERSHEY_SIMPLEX, 0.55,
                cv::Scalar(30, 30, 30), 2, cv::LINE_AA);
    res.plot = canvas;
    return res;
}

}  // namespace numcalc
