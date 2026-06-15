#pragma once

#include "framework.h"
#include "resource.h"

// "About" dialog: shows the CMake project version plus copyright / contact.
class CAboutDlg : public CDialogEx {
public:
    CAboutDlg();
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual BOOL OnInitDialog();
    DECLARE_MESSAGE_MAP()
};
