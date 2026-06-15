#pragma once

#include <string>
#include <opencv2/core.hpp>

namespace numcalc {

// Unified return type for every demonstration algorithm.
//   title  : short name shown as a heading
//   report : multi-line human readable text (inputs, steps, outputs)
//   plot   : optional BGR visualization (cv::Mat, may be empty)
struct NcResult {
    std::string title;
    std::string report;
    cv::Mat     plot;
};

}  // namespace numcalc
