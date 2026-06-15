
// numericalCalculaterDlg.h : 头文件
//

#pragma once
#include  <opencv2/opencv.hpp>
#include  <opencv/cv.h>
#include  "CvvImage.h"

#include "calcTR2.h"
#include "calcTR1.h"

// CnumericalCalculaterDlg 对话框
class CnumericalCalculaterDlg : public CDialogEx
{
// 构造
public:
	CnumericalCalculaterDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_numericalCalculater_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1_trans2YUV();
	afx_msg void OnBnClickedButton2_test_binary_io_align_problm();
	afx_msg void OnBnClickedButton3_study_fundamentalMatrix();
	afx_msg void OnBnClickedButton4_optimization();
	afx_msg void OnBnClickedButton5_numerical_analysis();
	afx_msg void OnBnClickedButton6_numerical_opt_gaus_nuton();
	afx_msg void OnBnClickedButton7_mixmizeBinaryProduct();
	afx_msg void OnBnClickedButton8_function_fit_02();
	afx_msg void OnBnClickedButton9_optical_flow_method();
	afx_msg void OnBnClickedButton10_video2__383x383x3();
	afx_msg void OnBnClickedButton11_get2Frequence();
	afx_msg void OnBnClickedButton12_opencv_player_toobar();
	afx_msg void OnBnClickedButton13_oneChannels2Three();
	afx_msg void OnBnClickedButton14_dlibBayes();
	afx_msg void OnBnClickedButton15_dlibSvm();
	afx_msg void OnBnClickedButton16_videoObjectTrack();
	afx_msg void OnBnClickedButton17_dlib3dPointCloud();
	afx_msg void OnBnClickedButton18_dlibAssignment();
	afx_msg void OnBnClickedButton19_dlibMultiClassify();
	afx_msg void OnBnClickedButton20_dlibKMeanCluster();



	void DrawPicToHDC(IplImage *img, UINT ID);
	void DrawPicToHDC(Mat  src   , UINT ID);
	Mat  src;
	afx_msg void OnBnClickedButton21_tr01();
	afx_msg void OnBnClickedButton22_tr02();
};
