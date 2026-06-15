#pragma once

// Public entry points for the numerical calculation demonstrations.
// Each function executes a self-contained worked example and returns an
// NcResult that bundles a textual report and an optional visualization image.

#include <string>
#include "nc_result.h"

namespace numcalc {

// 0. Image format conversion to YUV (BGR -> YUV / I420 planar).
NcResult RunYuvConvert(const std::string& imagePath);

// 1. Matrix operations (Eigen): multiply, transpose, determinant, inverse,
//    eigenvalues and solving a linear system A x = b.
NcResult RunMatrixOps();

// 2. Probability: binomial / normal distributions and a Bayes example.
NcResult RunProbability();

// 3. Statistics: descriptive stats + least-squares linear regression.
NcResult RunStatistics();

// 4. Logistic regression classifier trained by gradient descent.
NcResult RunLogisticRegression();

// 5. K-Nearest-Neighbors classification with a decision-region map.
NcResult RunKnn();

// 6. ANN: a small multilayer perceptron solving the XOR problem.
NcResult RunAnn();

// 7. CNN: a single convolution + ReLU + pooling forward pass on an image.
NcResult RunCnn(const std::string& imagePath);

}  // namespace numcalc
