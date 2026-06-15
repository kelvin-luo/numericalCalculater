#include "numcalc.h"
#include "nc_plot.h"

#include <sstream>
#include <iomanip>
#include <Eigen/Dense>
#include <opencv2/core.hpp>

namespace numcalc {

namespace {
std::string Fmt(const Eigen::MatrixXd& m) {
    std::ostringstream os;
    os << std::fixed << std::setprecision(3);
    for (int r = 0; r < m.rows(); ++r) {
        os << "    [ ";
        for (int c = 0; c < m.cols(); ++c) {
            os << std::setw(9) << m(r, c) << (c + 1 < m.cols() ? ", " : " ");
        }
        os << "]\n";
    }
    return os.str();
}

cv::Mat ToCv(const Eigen::MatrixXd& m) {
    cv::Mat out(static_cast<int>(m.rows()), static_cast<int>(m.cols()), CV_64F);
    for (int r = 0; r < m.rows(); ++r)
        for (int c = 0; c < m.cols(); ++c) out.at<double>(r, c) = m(r, c);
    return out;
}
}  // namespace

NcResult RunMatrixOps() {
    NcResult res;
    res.title = "1. Matrix Operations (Eigen)";

    Eigen::MatrixXd A(3, 3);
    A << 4, 3, 2,
         1, 5, 7,
         6, 2, 9;
    Eigen::MatrixXd B(3, 3);
    B << 1, 0, 2,
         0, 3, 1,
         4, 1, 0;

    Eigen::MatrixXd C   = A * B;
    Eigen::MatrixXd At  = A.transpose();
    double det          = A.determinant();
    Eigen::MatrixXd inv = A.inverse();

    Eigen::Vector3d b(6, 4, 9);
    Eigen::Vector3d x = A.colPivHouseholderQr().solve(b);

    Eigen::EigenSolver<Eigen::MatrixXd> es(A);

    std::ostringstream os;
    os << "Matrix A =\n" << Fmt(A) << "\n";
    os << "Matrix B =\n" << Fmt(B) << "\n";
    os << "A * B =\n" << Fmt(C) << "\n";
    os << "A^T =\n" << Fmt(At) << "\n";
    os << "det(A) = " << std::fixed << std::setprecision(4) << det << "\n\n";
    os << "A^{-1} =\n" << Fmt(inv) << "\n";
    os << "Solve A x = b, b = (6, 4, 9)^T :\n";
    os << "    x = (" << std::setprecision(4) << x(0) << ", " << x(1) << ", "
       << x(2) << ")^T\n";
    os << "    check A x = (" << (A * x).transpose() << ")\n\n";
    os << "Eigenvalues of A:\n";
    for (int i = 0; i < es.eigenvalues().size(); ++i) {
        std::complex<double> ev = es.eigenvalues()(i);
        os << "    lambda_" << i << " = " << std::setprecision(4) << ev.real();
        if (std::abs(ev.imag()) > 1e-9) os << " + " << ev.imag() << "i";
        os << "\n";
    }
    res.report = os.str();
    res.plot = Plot::heatmap(ToCv(C), 60, "Heat-map of C = A * B");
    return res;
}

}  // namespace numcalc
