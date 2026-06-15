#include "numcalc.h"
#include "nc_plot.h"

#include <sstream>
#include <iomanip>
#include <cmath>
#include <vector>

namespace numcalc {

namespace {
double Factorial(int n) {
    double f = 1.0;
    for (int i = 2; i <= n; ++i) f *= i;
    return f;
}
double Binom(int n, int k, double p) {
    double c = Factorial(n) / (Factorial(k) * Factorial(n - k));
    return c * std::pow(p, k) * std::pow(1 - p, n - k);
}
double NormalPdf(double x, double mu, double sigma) {
    static const double kInvSqrt2Pi = 0.3989422804014327;
    double z = (x - mu) / sigma;
    return kInvSqrt2Pi / sigma * std::exp(-0.5 * z * z);
}
double NormalCdf(double x, double mu, double sigma) {
    return 0.5 * std::erfc(-(x - mu) / (sigma * std::sqrt(2.0)));
}
}  // namespace

NcResult RunProbability() {
    NcResult res;
    res.title = "2. Probability";

    std::ostringstream os;
    os << std::fixed << std::setprecision(4);

    // --- Binomial distribution -------------------------------------------
    const int n = 10;
    const double p = 0.4;
    std::vector<double> pmf(n + 1);
    os << "Binomial B(n=10, p=0.4)  P(X=k):\n";
    for (int k = 0; k <= n; ++k) {
        pmf[k] = Binom(n, k, p);
        os << "    k=" << std::setw(2) << k << "  P=" << pmf[k] << "\n";
    }
    os << "    E[X] = n*p = " << n * p << ",  Var[X] = n*p*(1-p) = "
       << n * p * (1 - p) << "\n\n";

    // --- Normal distribution ---------------------------------------------
    const double mu = 0.0, sigma = 1.0;
    os << "Standard Normal N(0,1):\n";
    os << "    pdf(0) = " << NormalPdf(0, mu, sigma) << "\n";
    os << "    P(X <= 1.0)  = " << NormalCdf(1.0, mu, sigma) << "\n";
    os << "    P(-1<=X<=1)  = "
       << NormalCdf(1, mu, sigma) - NormalCdf(-1, mu, sigma) << "\n";
    os << "    P(-2<=X<=2)  = "
       << NormalCdf(2, mu, sigma) - NormalCdf(-2, mu, sigma) << "\n\n";

    // --- Bayes' theorem example ------------------------------------------
    // Disease prevalence 1%, test sensitivity 99%, specificity 95%.
    double prior = 0.01, sens = 0.99, spec = 0.95;
    double pPos = sens * prior + (1 - spec) * (1 - prior);
    double posterior = sens * prior / pPos;
    os << "Bayes example (medical test):\n";
    os << "    prior P(D)=0.01, sensitivity=0.99, specificity=0.95\n";
    os << "    P(positive) = " << pPos << "\n";
    os << "    P(D | positive) = " << posterior << "\n";
    res.report = os.str();

    // --- Visualization: binomial PMF + normal pdf curve ------------------
    Plot plot;
    plot.title("Binomial PMF (bars) & Normal pdf (curve)")
        .labels("x", "probability");
    plot.histogram(pmf, cv::Scalar(200, 140, 60));
    std::vector<double> xs, ys;
    for (double x = -0.5; x <= n + 0.5; x += 0.1) {
        xs.push_back(x);
        ys.push_back(NormalPdf(x, n * p, std::sqrt(n * p * (1 - p))));
    }
    plot.curve(xs, ys, cv::Scalar(40, 40, 220), "Normal approx");
    res.plot = plot.render();
    return res;
}

}  // namespace numcalc
