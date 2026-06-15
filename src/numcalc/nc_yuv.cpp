#include "numcalc.h"
#include "nc_plot.h"

#include <sstream>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

namespace numcalc {

namespace {
cv::Mat LoadOrSynthesize(const std::string& path, bool& synth) {
    cv::Mat img = cv::imread(path, cv::IMREAD_COLOR);
    synth = img.empty();
    if (synth) {
        img = cv::Mat(240, 320, CV_8UC3);
        for (int r = 0; r < img.rows; ++r)
            for (int c = 0; c < img.cols; ++c)
                img.at<cv::Vec3b>(r, c) =
                    cv::Vec3b(c % 256, r % 256, (r + c) % 256);
    }
    return img;
}
}  // namespace

// Convert a BGR image to YUV (and to planar I420), dump the I420 buffer to the
// output directory and visualize the Y / U / V planes.
NcResult RunYuvConvert(const std::string& imagePath) {
    NcResult res;
    res.title = "0. Image format conversion to YUV";

    bool synth = false;
    cv::Mat bgr = LoadOrSynthesize(imagePath, synth);
    // Keep the demo light: cap the long side at 480 px.
    if (std::max(bgr.cols, bgr.rows) > 480) {
        double s = 480.0 / std::max(bgr.cols, bgr.rows);
        cv::resize(bgr, bgr, cv::Size(), s, s);
    }
    // I420 needs even dimensions.
    if (bgr.cols % 2) bgr = bgr(cv::Rect(0, 0, bgr.cols - 1, bgr.rows)).clone();
    if (bgr.rows % 2) bgr = bgr(cv::Rect(0, 0, bgr.cols, bgr.rows - 1)).clone();

    cv::Mat yuv;
    cv::cvtColor(bgr, yuv, cv::COLOR_BGR2YUV);
    std::vector<cv::Mat> ch;
    cv::split(yuv, ch);  // ch[0]=Y, ch[1]=U, ch[2]=V

    // Planar I420 (YUV420p): full-res Y then quarter-res U and V.
    cv::Mat i420;
    cv::cvtColor(bgr, i420, cv::COLOR_BGR2YUV_I420);

    // Write the raw I420 buffer to PROJECT_DIR/output (relative to the exe).
    std::string outDir = "../output";
    std::error_code ec;
    std::filesystem::create_directories(outDir, ec);
    std::string yuvPath = outDir + "/converted_i420.yuv";
    std::ofstream ofs(yuvPath, std::ios::binary);
    bool wrote = false;
    if (ofs) {
        ofs.write(reinterpret_cast<const char*>(i420.data),
                  static_cast<std::streamsize>(i420.total() * i420.elemSize()));
        wrote = ofs.good();
    }

    std::ostringstream os;
    os << "Input: " << (synth ? "(synthetic gradient - sample image not found)"
                              : imagePath)
       << "\n";
    os << "Resolution: " << bgr.cols << " x " << bgr.rows << "\n\n";
    os << "BGR -> YUV (cv::COLOR_BGR2YUV), channels Y/U/V split.\n";
    os << "BGR -> I420 planar (YUV420p), size = " << i420.cols << " x "
       << i420.rows << " (bytes = " << i420.total() << ").\n\n";
    cv::Scalar meanY = cv::mean(ch[0]);
    cv::Scalar meanU = cv::mean(ch[1]);
    cv::Scalar meanV = cv::mean(ch[2]);
    os << std::fixed << std::setprecision(2);
    os << "Mean Y = " << meanY[0] << ",  Mean U = " << meanU[0]
       << ",  Mean V = " << meanV[0] << "\n\n";
    os << (wrote ? "Saved raw I420 to: " : "Failed to save I420 to: ")
       << yuvPath << "\n";
    res.report = os.str();

    // Visualization: original | Y | U | V as a 2x2 grid.
    auto label = [](cv::Mat m, const std::string& t) {
        cv::putText(m, t, {6, 20}, cv::FONT_HERSHEY_SIMPLEX, 0.6,
                    {0, 255, 0}, 2, cv::LINE_AA);
        return m;
    };
    cv::Mat yC, uC, vC;
    cv::cvtColor(ch[0], yC, cv::COLOR_GRAY2BGR);
    cv::cvtColor(ch[1], uC, cv::COLOR_GRAY2BGR);
    cv::cvtColor(ch[2], vC, cv::COLOR_GRAY2BGR);
    cv::Mat o0 = label(bgr.clone(), "BGR");
    cv::Mat o1 = label(yC, "Y");
    cv::Mat o2 = label(uC, "U");
    cv::Mat o3 = label(vC, "V");
    cv::Mat top, bottom, grid;
    cv::hconcat(o0, o1, top);
    cv::hconcat(o2, o3, bottom);
    cv::vconcat(top, bottom, grid);
    res.plot = grid;
    return res;
}

}  // namespace numcalc
