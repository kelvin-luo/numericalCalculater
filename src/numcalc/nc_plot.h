#pragma once

#include <string>
#include <vector>
#include <opencv2/core.hpp>

namespace numcalc {

// Lightweight OpenCV based plotting helpers used for data visualization.
// All helpers return a BGR cv::Mat that can be blitted to a Windows HDC.
class Plot {
public:
    Plot(int width = 760, int height = 460,
         const cv::Scalar& background = cv::Scalar(255, 255, 255));

    // Set axis title and labels (optional).
    Plot& title(const std::string& t);
    Plot& labels(const std::string& xLabel, const std::string& yLabel);

    // Draw a line curve from y values (x is the sample index) or x/y pairs.
    Plot& curve(const std::vector<double>& y, const cv::Scalar& color,
                const std::string& legend = "");
    Plot& curve(const std::vector<double>& x, const std::vector<double>& y,
                const cv::Scalar& color, const std::string& legend = "");

    // Draw a scatter of 2D points, optionally colored by integer class label.
    Plot& scatter(const std::vector<cv::Point2d>& pts,
                  const std::vector<int>& labels = {},
                  int radius = 4);

    // Draw a vertical-bar histogram from already-binned counts.
    Plot& histogram(const std::vector<double>& counts,
                    const cv::Scalar& color = cv::Scalar(180, 120, 40));

    // Render the finished canvas.
    cv::Mat render();

    // Convenience: draw a matrix as a colored heat-map image.
    static cv::Mat heatmap(const cv::Mat& m, int cellSize = 48,
                           const std::string& title = "");

private:
    void ensureBounds(const std::vector<double>& xs,
                      const std::vector<double>& ys);
    cv::Point2i toPixel(double x, double y) const;

    cv::Mat canvas_;
    int width_, height_;
    int marginL_ = 60, marginR_ = 20, marginT_ = 40, marginB_ = 44;
    std::string title_, xLabel_, yLabel_;
    double xMin_ = 0, xMax_ = 1, yMin_ = 0, yMax_ = 1;
    bool boundsSet_ = false;
    int legendCount_ = 0;
    bool axesDrawn_ = false;

    void drawAxes();
};

}  // namespace numcalc
