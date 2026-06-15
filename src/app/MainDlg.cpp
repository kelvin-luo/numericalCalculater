#include "MainDlg.h"
#include "AboutDlg.h"
#include "MatDisplay.h"
#include "numcalc.h"
#include "nc_version.h"

#include <opencv2/imgcodecs.hpp>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IDM_ABOUTBOX 0x0010

namespace {
CString Widen(const std::string& s) {
    return CString(CA2W(s.c_str(), CP_UTF8));
}
std::string Narrow(const CString& s) {
    CT2A conv(s, CP_UTF8);
    return std::string(static_cast<const char*>(conv));
}
// Convert "\n" report text to CRLF so the multiline edit shows line breaks.
CString ToCrLf(const std::string& s) {
    CString w = Widen(s);
    w.Replace(_T("\n"), _T("\r\n"));
    return w;
}
}  // namespace

CMainDlg::CMainDlg(CWnd* pParent) : CDialogEx(IDD_MAIN_DIALOG, pParent) {
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMainDlg::DoDataExchange(CDataExchange* pDX) {
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PIC, m_pic);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BTN_YUV, &CMainDlg::OnBnClickedYuv)
    ON_BN_CLICKED(IDC_BTN_MATRIX, &CMainDlg::OnBnClickedMatrix)
    ON_BN_CLICKED(IDC_BTN_PROB, &CMainDlg::OnBnClickedProb)
    ON_BN_CLICKED(IDC_BTN_STAT, &CMainDlg::OnBnClickedStat)
    ON_BN_CLICKED(IDC_BTN_LOGISTIC, &CMainDlg::OnBnClickedLogistic)
    ON_BN_CLICKED(IDC_BTN_KNN, &CMainDlg::OnBnClickedKnn)
    ON_BN_CLICKED(IDC_BTN_ANN, &CMainDlg::OnBnClickedAnn)
    ON_BN_CLICKED(IDC_BTN_CNN, &CMainDlg::OnBnClickedCnn)
    ON_BN_CLICKED(IDC_BTN_OPENIMG, &CMainDlg::OnBnClickedOpenImage)
    ON_BN_CLICKED(IDC_BTN_SAVEPLOT, &CMainDlg::OnBnClickedSavePlot)
    ON_BN_CLICKED(IDC_BTN_ABOUT, &CMainDlg::OnBnClickedAbout)
END_MESSAGE_MAP()

BOOL CMainDlg::OnInitDialog() {
    CDialogEx::OnInitDialog();

    // Add "About..." to the system menu.
    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu) {
        pSysMenu->AppendMenu(MF_SEPARATOR);
        pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, _T("About..."));
    }

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    CString caption;
    caption.Format(_T("Numerical Calculator  v%S"), NC_VERSION_STRING);
    SetWindowText(caption);

    // Beautified title font + background brushes.
    m_titleFont.CreateFont(22, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                           DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                           CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                           DEFAULT_PITCH | FF_SWISS, _T("Segoe UI"));
    if (GetDlgItem(IDC_STATIC_TITLE))
        GetDlgItem(IDC_STATIC_TITLE)->SetFont(&m_titleFont);

    m_bkBrush.CreateSolidBrush(RGB(245, 247, 250));

    m_imagePath = DefaultImagePath();
    SetDlgItemText(IDC_EDIT_PATH, m_imagePath);

    SetDlgItemText(IDC_EDIT_REPORT,
                   _T("Welcome to Numerical Calculator.\r\n\r\n")
                   _T("Click any method on the left to run a worked example. ")
                   _T("Results appear here and the chart is drawn on the right.\r\n\r\n")
                   _T("Methods: 0 YUV  1 Matrix  2 Probability  3 Statistics  ")
                   _T("4 Logistic  5 k-NN  6 ANN  7 CNN"));
    return TRUE;
}

CString CMainDlg::DefaultImagePath() const {
    // Prefer the sample copied into ./input ; fall back to the dev path.
    const TCHAR* candidates[] = {
        _T("input\\people.jpeg"),
        _T("..\\msvc_release\\input\\people.jpeg")};
    for (const TCHAR* c : candidates) {
        if (::GetFileAttributes(c) != INVALID_FILE_ATTRIBUTES) return CString(c);
    }
    return _T("input\\people.jpeg");
}

void CMainDlg::ShowResult(const numcalc::NcResult& r) {
    SetDlgItemText(IDC_EDIT_REPORT, ToCrLf(r.title + "\n" +
                                           std::string(50, '-') + "\n" +
                                           r.report));
    m_plot = r.plot;
    m_pic.SetMat(r.plot);
}

void CMainDlg::OnBnClickedYuv() {
    CString p; GetDlgItemText(IDC_EDIT_PATH, p);
    ShowResult(numcalc::RunYuvConvert(Narrow(p)));
}
void CMainDlg::OnBnClickedMatrix() { ShowResult(numcalc::RunMatrixOps()); }
void CMainDlg::OnBnClickedProb() { ShowResult(numcalc::RunProbability()); }
void CMainDlg::OnBnClickedStat() { ShowResult(numcalc::RunStatistics()); }
void CMainDlg::OnBnClickedLogistic() {
    ShowResult(numcalc::RunLogisticRegression());
}
void CMainDlg::OnBnClickedKnn() { ShowResult(numcalc::RunKnn()); }
void CMainDlg::OnBnClickedAnn() { ShowResult(numcalc::RunAnn()); }
void CMainDlg::OnBnClickedCnn() {
    CString p; GetDlgItemText(IDC_EDIT_PATH, p);
    ShowResult(numcalc::RunCnn(Narrow(p)));
}

void CMainDlg::OnBnClickedOpenImage() {
    CFileDialog dlg(TRUE, nullptr, nullptr,
                    OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
                    _T("Images|*.jpg;*.jpeg;*.png;*.bmp|All Files|*.*||"),
                    this);
    if (dlg.DoModal() == IDOK) {
        m_imagePath = dlg.GetPathName();
        SetDlgItemText(IDC_EDIT_PATH, m_imagePath);
    }
}

void CMainDlg::OnBnClickedSavePlot() {
    if (m_plot.empty()) {
        AfxMessageBox(_T("No chart to save yet. Run a method first."));
        return;
    }
    CFileDialog dlg(FALSE, _T("png"), _T("plot.png"),
                    OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
                    _T("PNG|*.png|JPEG|*.jpg||"), this);
    if (dlg.DoModal() == IDOK) {
        std::string path = Narrow(dlg.GetPathName());
        if (cv::imwrite(path, m_plot))
            AfxMessageBox(_T("Chart saved."));
        else
            AfxMessageBox(_T("Failed to save chart."));
    }
}

void CMainDlg::OnBnClickedAbout() {
    CAboutDlg dlg;
    dlg.DoModal();
}

void CMainDlg::OnSysCommand(UINT nID, LPARAM lParam) {
    if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
        CAboutDlg dlg;
        dlg.DoModal();
    } else {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

void CMainDlg::OnPaint() {
    CDialogEx::OnPaint();
}

HCURSOR CMainDlg::OnQueryDragIcon() {
    return static_cast<HCURSOR>(m_hIcon);
}

HBRUSH CMainDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
    if (nCtlColor == CTLCOLOR_DLG) {
        return m_bkBrush;
    }
    if (nCtlColor == CTLCOLOR_STATIC) {
        pDC->SetBkMode(TRANSPARENT);
        if (pWnd && pWnd->GetDlgCtrlID() == IDC_STATIC_TITLE)
            pDC->SetTextColor(RGB(30, 80, 160));
        else
            pDC->SetTextColor(RGB(40, 40, 40));
        return m_bkBrush;
    }
    return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
}
