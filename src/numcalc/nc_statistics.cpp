#include "numcalc.h"
#include "nc_plot.h"

#include <sstream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <vector>

namespace numcalc {

NcResult RunStatistics() {
    NcResult res;
    res.title = "3. Statistics";

    // A small bivariate sample (x, y) with a roughly linear trend.
    std::vector<double> x = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<double> y = {2.1, 3.9, 6.2, 7.8, 10.3, 11.8, 14.1, 16.2, 17.9, 20.4};
    const size_t n = x.size();

    auto mean = [](const std::vector<double>& v) {
        return std::accumulate(v.begin(), v.end(), 0.0) / v.size();
    };
    double mx = mean(x), my = mean(y);

    double varx = 0, vary = 0, cov = 0;
    for (size_t i = 0; i < n; ++i) {
        varx += (x[i] - mx) * (x[i] - mx);
        vary += (y[i] - my) * (y[i] - my);
        cov  += (x[i] - mx) * (y[i] - my);
    }
    varx /= (n - 1); vary /= (n - 1); cov /= (n - 1);
    double sx = std::sqrt(varx), sy = std::sqrt(vary);
    double corr = cov / (sx * sy);

    // Least-squares line y = a*x + b.
    double a = cov / varx;
    double b = my - a * mx;

    std::vector<double> ysorted = y;
    std::sort(ysorted.begin(), ysorted.end());
    double median = (n % 2) ? ysorted[n / 2]
                            : 0.5 * (ysorted[n / 2 - 1] + ysorted[n / 2]);

    std::ostringstream os;
    os << std::fixed << std::setprecision(4);
    os << "Sample size n = " << n << "\n";
    os << "x mean = " << mx << ",  y mean = " << my << "\n";
    os << "y median = " << median << "\n";
    os << "Var(x) = " << varx << ",  Std(x) = " << sx << "\n";
    os << "Var(y) = " << vary << ",  Std(y) = " << sy << "\n";
    os << "Cov(x,y) = " << cov << "\n";
    os << "Pearson correlation r = " << corr << "\n\n";
    os << "Least-squares regression:\n";
    os << "    y = " << a << " * x + " << b << "\n";
    os << "    (slope a = " << a << ", intercept b = " << b << ")\n";
    res.report = os.str();

    // Visualization: scatter of samples + fitted regression line.
    Plot plot;
    plot.title("Linear regression").labels("x", "y");
    std::vector<cv::Point2d> pts;
    for (size_t i = 0; i < n; ++i) pts.emplace_back(x[i], y[i]);
    plot.scatter(pts, {}, 5);
    std::vector<double> lx = {x.front(), x.back()};
    std::vector<double> ly = {a * x.front() + b, a * x.back() + b};
    plot.curve(lx, ly, cv::Scalar(40, 40, 220), "fit");
    res.plot = plot.render();
    return res;
}

}  // namespace numcalc
