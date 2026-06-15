#include <gtest/gtest.h>
#include "numcalc.h"

#include <Eigen/Dense>
#include <cmath>

using namespace numcalc;

// Every demo must produce a non-empty title, report and plot image.
TEST(NumCalc, AllDemosProduceOutput) {
    std::vector<NcResult> results = {
        RunMatrixOps(),      RunProbability(),  RunStatistics(),
        RunLogisticRegression(), RunKnn(),      RunAnn(),
        RunYuvConvert(""),   RunCnn("")};
    for (const auto& r : results) {
        EXPECT_FALSE(r.title.empty());
        EXPECT_FALSE(r.report.empty());
        EXPECT_FALSE(r.plot.empty()) << "empty plot for: " << r.title;
    }
}

// Sanity-check the Eigen linear algebra used by RunMatrixOps.
TEST(NumCalc, EigenSolveIsConsistent) {
    Eigen::Matrix3d A;
    A << 4, 3, 2, 1, 5, 7, 6, 2, 9;
    Eigen::Vector3d b(6, 4, 9);
    Eigen::Vector3d x = A.colPivHouseholderQr().solve(b);
    EXPECT_LT((A * x - b).norm(), 1e-9);
}

// The logistic-regression demo should separate the two blobs well.
TEST(NumCalc, LogisticReportsHighAccuracy) {
    NcResult r = RunLogisticRegression();
    auto pos = r.report.find("accuracy = ");
    ASSERT_NE(pos, std::string::npos);
    double acc = std::atof(r.report.c_str() + pos + 11);
    EXPECT_GT(acc, 85.0);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
