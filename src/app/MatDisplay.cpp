#include "MatDisplay.h"
#include <opencv2/imgproc.hpp>

void DrawMatToDC(CDC* pDC, const CRect& dst, const cv::Mat& mat) {
    if (!pDC || mat.empty() || dst.Width() <= 0 || dst.Height() <= 0) return;

    cv::Mat bgr;
    if (mat.channels() == 1) {
        cv::cvtColor(mat, bgr, cv::COLOR_GRAY2BGR);
    } else if (mat.channels() == 4) {
        cv::cvtColor(mat, bgr, cv::COLOR_BGRA2BGR);
    } else {
        bgr = mat;
    }
    if (bgr.depth() != CV_8U) bgr.convertTo(bgr, CV_8U);

    // Fit while preserving aspect ratio.
    double sx = static_cast<double>(dst.Width()) / bgr.cols;
    double sy = static_cast<double>(dst.Height()) / bgr.rows;
    double s = (std::min)(sx, sy);
    int w = (std::max)(1, static_cast<int>(bgr.cols * s));
    int h = (std::max)(1, static_cast<int>(bgr.rows * s));
    int ox = dst.left + (dst.Width() - w) / 2;
    int oy = dst.top + (dst.Height() - h) / 2;

    // DIB rows must be 4-byte aligned; build a padded buffer.
    cv::Mat show;
    if (bgr.cols % 4 != 0) {
        cv::copyMakeBorder(bgr, show, 0, 0, 0, 4 - (bgr.cols % 4),
                           cv::BORDER_CONSTANT, cv::Scalar(255, 255, 255));
    } else {
        show = bgr;
    }

    BITMAPINFO bi;
    ZeroMemory(&bi, sizeof(bi));
    bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bi.bmiHeader.biWidth = show.cols;
    bi.bmiHeader.biHeight = -show.rows;  // top-down
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 24;
    bi.bmiHeader.biCompression = BI_RGB;

    int oldMode = pDC->SetStretchBltMode(HALFTONE);
    ::StretchDIBits(pDC->GetSafeHdc(), ox, oy, w, h, 0, 0, bgr.cols, bgr.rows,
                    show.data, &bi, DIB_RGB_COLORS, SRCCOPY);
    pDC->SetStretchBltMode(oldMode);
}

BEGIN_MESSAGE_MAP(CMatStatic, CStatic)
    ON_WM_PAINT()
END_MESSAGE_MAP()

void CMatStatic::SetMat(const cv::Mat& m) {
    m_mat = m.clone();
    if (GetSafeHwnd()) Invalidate();
}

void CMatStatic::OnPaint() {
    CPaintDC dc(this);
    CRect rc;
    GetClientRect(&rc);
    dc.FillSolidRect(&rc, RGB(252, 252, 252));
    dc.Draw3dRect(&rc, RGB(200, 205, 212), RGB(200, 205, 212));
    if (m_mat.empty()) {
        dc.SetTextColor(RGB(150, 150, 150));
        dc.SetBkMode(TRANSPARENT);
        dc.DrawText(_T("Visualization will appear here"), &rc,
                    DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    } else {
        CRect inner = rc;
        inner.DeflateRect(3, 3);
        DrawMatToDC(&dc, inner, m_mat);
    }
}
