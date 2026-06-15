#include "numcalc.h"
#include "nc_plot.h"

#include <sstream>
#include <iomanip>
#include <vector>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

namespace numcalc {

namespace {
// Load the image, or synthesize a simple test pattern if it is missing.
cv::Mat LoadOrSynthesize(const std::string& path, bool& synthesized) {
    cv::Mat img = cv::imread(path, cv::IMREAD_COLOR);
    synthesized = img.empty();
    if (synthesized) {
        img = cv::Mat(256, 256, CV_8UC3, cv::Scalar(40, 40, 40));
        cv::rectangle(img, {40, 40}, {150, 150}, {220, 180, 80}, -1);
        cv::circle(img, {180, 180}, 50, {80, 120, 230}, -1);
        cv::line(img, {0, 0}, {255, 255}, {255, 255, 255}, 3);
    }
    return img;
}

// Normalize a float feature map to a displayable 8-bit single channel.
cv::Mat ToDisplay(const cv::Mat& f) {
    cv::Mat out;
    cv::normalize(f, out, 0, 255, cv::NORM_MINMAX);
    out.convertTo(out, CV_8U);
    cv::Mat color;
    cv::applyColorMap(out, color, cv::COLORMAP_VIRIDIS);
    return color;
}
}  // namespace

// One convolution layer (4 fixed kernels) + ReLU + 2x2 max-pooling.
NcResult RunCnn(const std::string& imagePath) {
    NcResult res;
    res.title = "7. CNN - convolution + ReLU + pooling";

    bool synth = false;
    cv::Mat bgr = LoadOrSynthesize(imagePath, synth);
    cv::Mat gray;
    cv::cvtColor(bgr, gray, cv::COLOR_BGR2GRAY);
    cv::resize(gray, gray, cv::Size(128, 128));
    cv::Mat input;
    gray.convertTo(input, CV_32F, 1.0 / 255.0);

    // Four classic edge/blur kernels acting as conv filters.
    std::vector<std::pair<std::string, cv::Matx33f>> kernels = {
        {"Sobel-X", {-1, 0, 1, -2, 0, 2, -1, 0, 1}},
        {"Sobel-Y", {-1, -2, -1, 0, 0, 0, 1, 2, 1}},
        {"Laplacian", {0, 1, 0, 1, -4, 1, 0, 1, 0}},
        {"Blur", {1 / 9.f, 1 / 9.f, 1 / 9.f, 1 / 9.f, 1 / 9.f, 1 / 9.f, 1 / 9.f,
                  1 / 9.f, 1 / 9.f}}};

    std::ostringstream os;
    os << "Input: " << (synth ? "(synthetic test pattern - sample image not found)"
                              : imagePath)
       << "\n";
    os << "Pipeline: gray 128x128 -> conv 3x3 (4 filters) -> ReLU -> maxpool 2x2\n";
    os << "Output feature maps: 64x64 each\n\n";

    std::vector<cv::Mat> tiles;
    for (auto& kv : kernels) {
        cv::Mat conv;
        cv::filter2D(input, conv, CV_32F, cv::Mat(kv.second));
        cv::max(conv, 0.0, conv);  // ReLU
        // 2x2 max pooling.
        cv::Mat pooled(conv.rows / 2, conv.cols / 2, CV_32F);
        for (int r = 0; r < pooled.rows; ++r)
            for (int c = 0; c < pooled.cols; ++c) {
                float m = 0;
                for (int dr = 0; dr < 2; ++dr)
                    for (int dc = 0; dc < 2; ++dc)
                        m = std::max(m, conv.at<float>(2 * r + dr, 2 * c + dc));
                pooled.at<float>(r, c) = m;
            }
        double mn, mx;
        cv::minMaxLoc(pooled, &mn, &mx);
        os << "    filter " << std::left << std::setw(10) << kv.first
           << " activation range [" << std::fixed << std::setprecision(3) << mn
           << ", " << mx << "]\n";
        cv::Mat disp = ToDisplay(pooled);
        cv::resize(disp, disp, cv::Size(160, 160), 0, 0, cv::INTER_NEAREST);
        cv::putText(disp, kv.first, {6, 18}, cv::FONT_HERSHEY_SIMPLEX, 0.5,
                    {255, 255, 255}, 1, cv::LINE_AA);
        tiles.push_back(disp);
    }
    res.report = os.str();

    // Compose a 2x2 grid of the feature maps + the input thumbnail.
    cv::Mat top, bottom, grid;
    cv::hconcat(tiles[0], tiles[1], top);
    cv::hconcat(tiles[2], tiles[3], bottom);
    cv::vconcat(top, bottom, grid);
    res.plot = grid;
    return res;
}

}  // namespace numcalc
