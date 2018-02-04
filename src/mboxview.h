// mboxview.h : main header file for the mboxview application
//

#if !defined(AFX_mboxview_H__01D75A85_01DB_4CC8_A34E_AC20E309168D__INCLUDED_)
#define AFX_mboxview_H__01D75A85_01DB_4CC8_A34E_AC20E309168D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "mainfrm.h"       // main symbols
#include "TextUtilities.h"

extern const char *sz_Software_mboxview;

/////////////////////////////////////////////////////////////////////////////
// CmboxviewApp:
// See mboxview.cpp for the implementation of this class
//

#define MAPPING_SIZE 268435456

class CmboxviewApp : public CWinApp
{
public:
	CmboxviewApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CmboxviewApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CmboxviewApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	afx_msg void MyMRUFileHandler(UINT i);
	DECLARE_MESSAGE_MAP()
};


extern CmboxviewApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_mboxview_H__01D75A85_01DB_4CC8_A34E_AC20E309168D__INCLUDED_)