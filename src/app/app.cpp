#include "app.h"
#include "MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CNumCalcApp, CWinApp)
END_MESSAGE_MAP()

CNumCalcApp::CNumCalcApp() {}

CNumCalcApp theApp;

BOOL CNumCalcApp::InitInstance() {
    INITCOMMONCONTROLSEX initCtrls;
    initCtrls.dwSize = sizeof(initCtrls);
    initCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&initCtrls);

    CWinApp::InitInstance();

    SetRegistryKey(_T("Aesthetic Company Limited"));

    CMainDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;  // exit the application rather than start the message pump
}
