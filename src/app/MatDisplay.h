#pragma once

#include "framework.h"
#include <opencv2/core.hpp>

// Blit a BGR (CV_8UC3) or gray (CV_8UC1) cv::Mat into a device context,
// scaled to fit the destination rectangle while keeping the aspect ratio.
void DrawMatToDC(CDC* pDC, const CRect& dst, const cv::Mat& mat);

// A static control that paints a cv::Mat (used as the visualization area).
class CMatStatic : public CStatic {
public:
    void SetMat(const cv::Mat& m);

protected:
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()

private:
    cv::Mat m_mat;
};
