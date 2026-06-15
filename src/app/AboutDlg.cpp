#include "AboutDlg.h"
#include "nc_version.h"

#include <string>

namespace {
CString Widen(const std::string& s) {
    return CString(CA2W(s.c_str(), CP_UTF8));
}
}  // namespace

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX) {}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog() {
    CDialogEx::OnInitDialog();

    SetWindowText(_T("About - ") _T(NC_PROJECT_NAME));

    CString title;
    title.Format(_T("%S"), NC_PROJECT_NAME);
    SetDlgItemText(IDC_ABOUT_TITLE, title);

    CString ver;
    ver.Format(_T("Version %S"), NC_VERSION_STRING);
    SetDlgItemText(IDC_ABOUT_VERSION, ver);

    SetDlgItemText(IDC_ABOUT_COPYRIGHT, Widen(NC_COPYRIGHT));
    SetDlgItemText(IDC_ABOUT_EMAIL,
                   _T("Email: ") + Widen(NC_CONTACT_EMAIL));
    SetDlgItemText(IDC_ABOUT_WECHAT,
                   _T("WeChat: ") + Widen(NC_CONTACT_WECHAT));
    return TRUE;
}
