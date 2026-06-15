#include "nc_plot.h"

#include <algorithm>
#include <cmath>
#include <opencv2/imgproc.hpp>

namespace numcalc {

namespace {
constexpr int kFont = cv::FONT_HERSHEY_SIMPLEX;
}

Plot::Plot(int width, int height, const cv::Scalar& background)
    : width_(width), height_(height) {
    canvas_ = cv::Mat(height_, width_, CV_8UC3, background);
}

Plot& Plot::title(const std::string& t) {
    title_ = t;
    return *this;
}

Plot& Plot::labels(const std::string& xLabel, const std::string& yLabel) {
    xLabel_ = xLabel;
    yLabel_ = yLabel;
    return *this;
}

void Plot::ensureBounds(const std::vector<double>& xs,
                        const std::vector<double>& ys) {
    if (xs.empty() || ys.empty()) return;
    double xmn = *std::min_element(xs.begin(), xs.end());
    double xmx = *std::max_element(xs.begin(), xs.end());
    double ymn = *std::min_element(ys.begin(), ys.end());
    double ymx = *std::max_element(ys.begin(), ys.end());
    if (!boundsSet_) {
        xMin_ = xmn; xMax_ = xmx; yMin_ = ymn; yMax_ = ymx;
        boundsSet_ = true;
    } else {
        xMin_ = std::min(xMin_, xmn); xMax_ = std::max(xMax_, xmx);
        yMin_ = std::min(yMin_, ymn); yMax_ = std::max(yMax_, ymx);
    }
    // Avoid a degenerate range.
    if (xMax_ - xMin_ < 1e-9) { xMax_ = xMin_ + 1.0; }
    if (yMax_ - yMin_ < 1e-9) { yMax_ = yMin_ + 1.0; }
}

cv::Point2i Plot::toPixel(double x, double y) const {
    double px = marginL_ + (x - xMin_) / (xMax_ - xMin_) *
                               (width_ - marginL_ - marginR_);
    double py = (height_ - marginB_) - (y - yMin_) / (yMax_ - yMin_) *
                                           (height_ - marginT_ - marginB_);
    return cv::Point2i(static_cast<int>(std::lround(px)),
                       static_cast<int>(std::lround(py)));
}

void Plot::drawAxes() {
    if (axesDrawn_) return;
    axesDrawn_ = true;
    const cv::Scalar axis(80, 80, 80), grid(225, 225, 225);
    cv::Point2i o(marginL_, height_ - marginB_);
    cv::Point2i xEnd(width_ - marginR_, height_ - marginB_);
    cv::Point2i yEnd(marginL_, marginT_);

    // Grid lines + tick labels (5 divisions on each axis).
    for (int i = 0; i <= 5; ++i) {
        double fx = xMin_ + (xMax_ - xMin_) * i / 5.0;
        double fy = yMin_ + (yMax_ - yMin_) * i / 5.0;
        cv::Point2i gx = toPixel(fx, yMin_);
        cv::Point2i gy = toPixel(xMin_, fy);
        cv::line(canvas_, cv::Point(gx.x, marginT_),
                 cv::Point(gx.x, height_ - marginB_), grid, 1);
        cv::line(canvas_, cv::Point(marginL_, gy.y),
                 cv::Point(width_ - marginR_, gy.y), grid, 1);
        char buf[32];
        std::snprintf(buf, sizeof(buf), "%.2g", fx);
        cv::putText(canvas_, buf, cv::Point(gx.x - 12, height_ - marginB_ + 18),
                    kFont, 0.38, axis, 1, cv::LINE_AA);
        std::snprintf(buf, sizeof(buf), "%.2g", fy);
        cv::putText(canvas_, buf, cv::Point(4, gy.y + 4), kFont, 0.38, axis, 1,
                    cv::LINE_AA);
    }

    cv::line(canvas_, o, xEnd, axis, 2, cv::LINE_AA);
    cv::line(canvas_, o, yEnd, axis, 2, cv::LINE_AA);

    if (!title_.empty()) {
        cv::putText(canvas_, title_, cv::Point(marginL_, marginT_ - 14), kFont,
                    0.6, cv::Scalar(30, 30, 30), 2, cv::LINE_AA);
    }
    if (!xLabel_.empty()) {
        cv::putText(canvas_, xLabel_,
                    cv::Point(width_ / 2 - 20, height_ - 8), kFont, 0.45,
                    cv::Scalar(60, 60, 60), 1, cv::LINE_AA);
    }
    if (!yLabel_.empty()) {
        cv::putText(canvas_, yLabel_, cv::Point(6, marginT_ - 16), kFont, 0.45,
                    cv::Scalar(60, 60, 60), 1, cv::LINE_AA);
    }
}

Plot& Plot::curve(const std::vector<double>& y, const cv::Scalar& color,
                  const std::string& legend) {
    std::vector<double> x(y.size());
    for (size_t i = 0; i < y.size(); ++i) x[i] = static_cast<double>(i);
    return curve(x, y, color, legend);
}

Plot& Plot::curve(const std::vector<double>& x, const std::vector<double>& y,
                  const cv::Scalar& color, const std::string& legend) {
    ensureBounds(x, y);
    drawAxes();
    for (size_t i = 1; i < x.size() && i < y.size(); ++i) {
        cv::line(canvas_, toPixel(x[i - 1], y[i - 1]), toPixel(x[i], y[i]),
                 color, 2, cv::LINE_AA);
    }
    if (!legend.empty()) {
        int ly = marginT_ + 14 + legendCount_ * 18;
        cv::line(canvas_, cv::Point(width_ - marginR_ - 120, ly),
                 cv::Point(width_ - marginR_ - 95, ly), color, 2, cv::LINE_AA);
        cv::putText(canvas_, legend, cv::Point(width_ - marginR_ - 90, ly + 4),
                    kFont, 0.4, cv::Scalar(40, 40, 40), 1, cv::LINE_AA);
        ++legendCount_;
    }
    return *this;
}

Plot& Plot::scatter(const std::vector<cv::Point2d>& pts,
                    const std::vector<int>& labels, int radius) {
    std::vector<double> xs, ys;
    xs.reserve(pts.size());
    ys.reserve(pts.size());
    for (const auto& p : pts) { xs.push_back(p.x); ys.push_back(p.y); }
    // Add a small padding to the bounds so points are not on the border.
    if (!xs.empty()) {
        double padx = (*std::max_element(xs.begin(), xs.end()) -
                       *std::min_element(xs.begin(), xs.end())) * 0.05 + 0.5;
        double pady = (*std::max_element(ys.begin(), ys.end()) -
                       *std::min_element(ys.begin(), ys.end())) * 0.05 + 0.5;
        xs.push_back(*std::min_element(xs.begin(), xs.end()) - padx);
        xs.push_back(*std::max_element(xs.begin(), xs.end()) + padx);
        ys.push_back(*std::min_element(ys.begin(), ys.end()) - pady);
        ys.push_back(*std::max_element(ys.begin(), ys.end()) + pady);
    }
    ensureBounds(xs, ys);
    drawAxes();
    const cv::Scalar palette[] = {
        {60, 76, 231}, {231, 156, 60}, {76, 175, 80},
        {156, 60, 231}, {60, 200, 200}, {120, 120, 120}};
    for (size_t i = 0; i < pts.size(); ++i) {
        int c = labels.empty() ? 0 : std::max(0, labels[i]) % 6;
        cv::circle(canvas_, toPixel(pts[i].x, pts[i].y), radius, palette[c], -1,
                   cv::LINE_AA);
    }
    return *this;
}

Plot& Plot::histogram(const std::vector<double>& counts,
                      const cv::Scalar& color) {
    if (counts.empty()) return *this;
    std::vector<double> xs = {0.0, static_cast<double>(counts.size())};
    std::vector<double> ys = {0.0, *std::max_element(counts.begin(),
                                                     counts.end())};
    ensureBounds(xs, ys);
    drawAxes();
    int n = static_cast<int>(counts.size());
    for (int i = 0; i < n; ++i) {
        cv::Point2i p0 = toPixel(i + 0.1, 0.0);
        cv::Point2i p1 = toPixel(i + 0.9, counts[i]);
        cv::rectangle(canvas_, p0, p1, color, -1, cv::LINE_AA);
        cv::rectangle(canvas_, p0, p1, cv::Scalar(90, 90, 90), 1, cv::LINE_AA);
    }
    return *this;
}

cv::Mat Plot::render() {
    if (!axesDrawn_) drawAxes();
    return canvas_;
}

cv::Mat Plot::heatmap(const cv::Mat& m, int cellSize, const std::string& title) {
    cv::Mat md;
    m.convertTo(md, CV_64F);
    double mn, mx;
    cv::minMaxLoc(md, &mn, &mx);
    int top = title.empty() ? 8 : 30;
    cv::Mat out(md.rows * cellSize + top + 4, md.cols * cellSize + 4, CV_8UC3,
                cv::Scalar(255, 255, 255));
    for (int r = 0; r < md.rows; ++r) {
        for (int c = 0; c < md.cols; ++c) {
            double v = md.at<double>(r, c);
            double t = (mx - mn < 1e-12) ? 0.5 : (v - mn) / (mx - mn);
            // Blue (low) -> red (high) gradient.
            cv::Scalar color(255 * (1 - t), 70, 255 * t);
            cv::Point p0(2 + c * cellSize, top + r * cellSize);
            cv::Point p1(2 + (c + 1) * cellSize, top + (r + 1) * cellSize);
            cv::rectangle(out, p0, p1, color, -1);
            cv::rectangle(out, p0, p1, cv::Scalar(240, 240, 240), 1);
            char buf[32];
            std::snprintf(buf, sizeof(buf), "%.2f", v);
            cv::Scalar txt = t > 0.55 ? cv::Scalar(255, 255, 255)
                                      : cv::Scalar(20, 20, 20);
            cv::putText(out, buf, cv::Point(p0.x + 4, p0.y + cellSize / 2 + 4),
                        kFont, 0.34, txt, 1, cv::LINE_AA);
        }
    }
    if (!title.empty()) {
        cv::putText(out, title, cv::Point(6, 20), kFont, 0.55,
                    cv::Scalar(30, 30, 30), 2, cv::LINE_AA);
    }
    return out;
}

}  // namespace numcalc
