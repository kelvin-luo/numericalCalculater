#pragma once

#include "framework.h"
#include "resource.h"

// Application object for numericalCalculater.
class CNumCalcApp : public CWinApp {
public:
    CNumCalcApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CNumCalcApp theApp;
