#pragma once

#include "framework.h"
#include "resource.h"
#include "nc_result.h"
#include "MatDisplay.h"
#include <opencv2/core.hpp>

// Main application dialog: a column of method buttons on the left, a
// visualization picture box on the right-top and a text report below it.
class CMainDlg : public CDialogEx {
public:
    CMainDlg(CWnd* pParent = nullptr);
    enum { IDD = IDD_MAIN_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

    afx_msg void OnBnClickedYuv();
    afx_msg void OnBnClickedMatrix();
    afx_msg void OnBnClickedProb();
    afx_msg void OnBnClickedStat();
    afx_msg void OnBnClickedLogistic();
    afx_msg void OnBnClickedKnn();
    afx_msg void OnBnClickedAnn();
    afx_msg void OnBnClickedCnn();
    afx_msg void OnBnClickedOpenImage();
    afx_msg void OnBnClickedSavePlot();
    afx_msg void OnBnClickedAbout();

    DECLARE_MESSAGE_MAP()

private:
    void ShowResult(const numcalc::NcResult& r);
    CString DefaultImagePath() const;

    HICON       m_hIcon = nullptr;
    cv::Mat     m_plot;           // current visualization (for "Save Chart")
    CMatStatic  m_pic;            // visualization control
    CString     m_imagePath;      // selected input image
    CFont       m_titleFont;
    CBrush      m_bkBrush;        // dialog background
};
